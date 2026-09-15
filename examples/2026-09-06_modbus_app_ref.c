/*
 * 2026-09-06 Modbus 03 应答模块参考实现（纯 C，PC 可编译）
 * ============================================================
 * 编译运行：gcc -Wall -o modbus_app 2026-09-06_modbus_app_ref.c && ./modbus_app
 *
 * 在整条链路中的位置（回顾数据流）：
 *   UART中断 → 收帧状态机(收齐一帧) → [CRC 层] → modbus_handle() 处理请求 → 应答发回
 *   本文件 = 链路的最末端：收到"完整请求帧"后怎么答。
 *
 * 设计决策（每个都要能讲出为什么）：
 * 1. 寄存器表 = RAM 公告板：regs[0]=40001 温度×10, regs[1]=40002 湿度×10
 *    - 谁写？SensorTask(AHT20 采集任务)  —— 本模块只读
 *    - 谁读？本模块组应答帧时读        —— 一源三出口之一
 *    - 为什么 ×10 用 uint16_t？温度 25.1℃ → 251，湿度 55.8% → 558，0~65535 装得下
 * 2. 应答复用请求 buffer：请求帧收在 buf 里，应答也写回 buf（省一块内存），
 *    返回值 = 应答帧长度（调用方拿长度去发串口）
 * 3. CRC 校验失败 → return 0 静默丢弃（Modbus 惯例：坏帧不答，不答异常）
 * 4. 异常应答：功能码 | 0x80 + 错误码（01=非法功能 02=非法地址 03=非法值）
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MY_ADDR    0x01      /* 本从站地址（单机教学：请求帧地址=01）*/
#define REG_COUNT  8         /* 公告板大小：40001~40008 */

static uint16_t regs[REG_COUNT];   /* 寄存器表（真机由 SensorTask 写，本模块只读）*/

enum {
    ERR_ILLEGAL_FUNCTION = 0x01,   /* 功能码不存在 */
    ERR_ILLEGAL_ADDRESS  = 0x02,   /* 寄存器地址越界 */
    ERR_ILLEGAL_VALUE    = 0x03
};

/* CRC16-Modbus（9.4 你已独立写过：初值 0xFFFF，多项式 0xA001，低字节先发）*/
static uint16_t crc16_modbus(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    uint16_t i;
    uint8_t  j;
    for (i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 8; j++)
            crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
    }
    return crc;
}

/*
 * 核心入口：处理一帧请求。
 * 入参：buf = 请求帧（收帧状态机收齐，CRC 还没验），len = 帧长
 * 出参：buf 被改写为应答帧；返回应答长度（0 = 无需应答/丢弃）
 */
uint16_t modbus_handle(uint8_t *buf, uint8_t len)
{
    uint8_t  func;
    uint16_t start, count, crc_rx, crc_calc, resp_len;
    uint16_t i;

    if (len < 8) return 0;                 /* 最短合法请求帧 = 1+1+2+2+2 = 8 字节 */

    /* ① 地址检查：不是叫我的帧 → 静默 */
    if (buf[0] != MY_ADDR) return 0;

    /* ② CRC 验证：帧尾两字节是 CRC（低字节在前），对前面所有字节重算比对 */
    crc_rx   = (uint16_t)buf[len-2] | ((uint16_t)buf[len-1] << 8);
    crc_calc = crc16_modbus(buf, len - 2);
    if (crc_calc != crc_rx) return 0;      /* 坏帧：静默丢弃，不答 */

    /* ③ 功能码分发 */
    func = buf[1];
    if (func != 0x03)                      /* 只支持 03 读保持寄存器 */
    {
        buf[1] = func | 0x80;              /* 异常应答：功能码最高位置 1 */
        buf[2] = ERR_ILLEGAL_FUNCTION;
        crc_calc = crc16_modbus(buf, 3);
        buf[3] = (uint8_t)(crc_calc & 0xFF);
        buf[4] = (uint8_t)(crc_calc >> 8);
        return 5;
    }

    /* ④ 参数解析 + 越界检查（寄存器地址 16 位：高字节 buf[2] 低字节 buf[3]）*/
    start = ((uint16_t)buf[2] << 8) | buf[3];
    count = ((uint16_t)buf[4] << 8) | buf[5];
    if (count == 0 || start + count > REG_COUNT)   /* 空读 / 越过公告板边界 */
    {
        buf[1] = 0x83;                     /* 0x03 | 0x80 = 0x83 */
        buf[2] = ERR_ILLEGAL_ADDRESS;
        crc_calc = crc16_modbus(buf, 3);
        buf[3] = (uint8_t)(crc_calc & 0xFF);
        buf[4] = (uint8_t)(crc_calc >> 8);
        return 5;
    }

    /* ⑤ 正常应答组帧：01 03 字节数 数据... CRC（寄存器高字节在前）*/
    buf[0] = MY_ADDR;
    buf[1] = 0x03;
    buf[2] = (uint8_t)(count * 2);         /* 数据字节数 = 寄存器数 × 2 */
    for (i = 0; i < count; i++)
    {
        buf[3 + i*2]     = (uint8_t)(regs[start + i] >> 8);   /* 高字节在前 */
        buf[3 + i*2 + 1] = (uint8_t)(regs[start + i] & 0xFF);
    }
    resp_len = 3 + count * 2;
    crc_calc = crc16_modbus(buf, resp_len);
    buf[resp_len]     = (uint8_t)(crc_calc & 0xFF);
    buf[resp_len + 1] = (uint8_t)(crc_calc >> 8);
    return resp_len + 2;
}

/* ========== 测试 main：模拟"状态机已收齐一帧 + SensorTask 已写公告板" ========== */
int main(void)
{
    /* 请求帧：01(地址) 03(读保持寄存器) 00 00(从 40001 起) 00 02(读 2 个) C4 0B(CRC) */
    uint8_t req[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
    uint16_t rlen, i;

    regs[0] = 251;    /* 模拟 SensorTask 写入：25.1℃ ×10 */
    regs[1] = 558;    /* 55.8% ×10 */

    rlen = modbus_handle(req, sizeof(req));
    printf("resp len=%d: ", rlen);
    for (i = 0; i < rlen; i++) printf("%02X ", req[i]);
    printf("\n");
    return 0;
}

/**
 * 2026-09-01 CRC16-Modbus 标准参考（Day1 手打用，注释详细版）
 *
 * 【背景】Modbus RTU 帧尾的 2 字节 CRC16 校验。
 *   CRC = 把整帧数据当"一个二进制大数"，除以固定多项式 0xA001，余数就是校验值。
 *   - 初值 0xFFFF：CRC 寄存器的起点（不是除数！除数是 0xA001）
 *   - 移出 1 才异或：右移时被移出的最低位是 1 → 右移后异或 0xA001
 *   - 逐位独立：异或/移位不涉及进位，可以安全地一位一位算（位运算三件套实战）
 *   - 数值现算不协商：发方收方各自算，结果一致 = 数据没坏（算法才协商）
 *
 * 【调用】crc16_modbus(数据指针, 长度) —— len = 帧中"除 CRC 外的字节数"
 *   请求帧 01 03 00 00 00 02 → len=6 → 得 C4 0B（低字节在前，C4 先发）
 *
 * 【记忆口诀】初值 FFFF，字节异或，八次右移，移 1 异或 A001。
 */
#include <stdio.h>
#include <stdint.h>

/**
 * @brief  计算 Modbus-RTU CRC16
 * @param  data : 帧数据指针（不含 CRC 字段）
 * @param  len  : 帧数据长度（字节数）
 * @retval 16 位 CRC 值（低字节在前发送）
 */
uint16_t crc16_modbus(uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFF;          /* ① 初值：CRC 寄存器装满 1（不是除数！）*/

    for (uint16_t i = 0; i < len; i++)   /* 外层：逐个字节处理 */
    {
        crc ^= data[i];             /* ② 混合：当前字节"揉进"CRC（异或=逐位混合，
                                         每个字节的信息都留下痕迹）*/

        for (uint8_t j = 0; j < 8; j++)  /* 内层：每个字节处理 8 位 */
        {
            if (crc & 0x0001)       /* ③ 看最低位：是 1？*/
            {
                crc = (crc >> 1) ^ 0xA001;  /* 右移 1 位 + 异或多项式（"除法"）*/
            }
            else
            {
                crc >>= 1;          /* 最低位是 0 → 只右移 */
            }
            /* ⚠️ 踩坑记忆：else 分支必须是 crc >>= 1（赋值！）
               写成 crc >> 1 只是"算了不存"，CRC 永远不变 → 结果全错 */
        }
    }

    return crc;                     /* ④ 出锅：余数 = CRC */
}

int main(void)
{
    /* 标准测试：上位机"读 1 号设备 2 个寄存器从 0x0000 起"的帧头 */
    uint8_t frame[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02};
    uint16_t crc = crc16_modbus(frame, 6);   /* len=6：除 CRC 外的字节数 */

    /* Modbus 发送顺序：低字节在前，所以先打印 crc & 0xFF 再打印 crc >> 8 */
    printf("CRC = %02X %02X (expect C4 0B)\n", crc & 0xFF, crc >> 8);

    /* 自测：0x01 单字节 → 期望 80 7E（手算验证用的标准值）*/
    uint8_t one[] = {0x01};
    uint16_t c1 = crc16_modbus(one, 1);
    printf("0x01 -> %02X %02X (expect 80 7E)\n", c1 & 0xFF, c1 >> 8);

    return 0;
}

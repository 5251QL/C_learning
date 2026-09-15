/*
 * 2026-09-05 Modbus RTU 收帧状态机参考实现（纯 C，PC 可编译）
 * ============================================================
 * 编译运行：gcc -Wall -o modbus_rx 2026-09-05_modbus_rx_ref.c && ./modbus_rx
 *
 * 设计说明：
 * - 本文件模拟"串口中断喂字节 + tick 计时"环境，验证状态机逻辑
 * - 真机上：modbus_rx_byte() 在 UART 中断回调里调用；modbus_rx_tick()
 *   在 SysTick/定时器里每 1ms 调用一次
 * - 帧尾判据：字节间隔 > 10ms（教学值；真 Modbus 是 3.5 字符 ≈ 4ms @9600）
 * - CRC 校验不在本文件（下一步做）——帧"收齐"≠帧"有效"，坏帧由 CRC 层丢弃
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ========== 状态机数据（全局，供中断/任务共享）========== */
#define RX_BUF_MAX 64

static uint8_t  s_buf[RX_BUF_MAX];   /* 收帧缓冲：字节按序存放 */
static uint8_t  s_len;               /* 已收字节数 */
static uint8_t  s_state;             /* 0 = IDLE 空闲, 1 = RECV 收帧中 */
static uint8_t  s_frame_ready;       /* 1 = 一帧收齐，等处理（置标志，任务消费）*/
static uint32_t s_last_tick;         /* 最后收到字节的 tick 值 */
static uint32_t s_tick;              /* 模拟系统 tick（每 ms +1）*/

#define FRAME_GAP_MS 10              /* 字节间隔超时阈值(ms)，教学值 */

/* ========== 接口 1：喂一个字节（真机在 UART 中断回调里调用）========== */
void modbus_rx_byte(uint8_t b)
{
    if (s_state == 0)                /* IDLE：帧的第一个字节 */
    {
        s_len = 0;                   /* 清空缓冲，重新计数 */
        s_state = 1;                 /* 进入收帧状态 */
        s_frame_ready = 0;           /* 清"帧完成"标志 */
    }

    if (s_len < RX_BUF_MAX)          /* 防溢出：缓冲满了就丢（异常保护）*/
    {
        s_buf[s_len++] = b;          /* 字节按序入缓冲 */
    }
    s_last_tick = s_tick;            /* 记录"最后活跃时刻"（超时判据）*/
}

/* ========== 接口 2：tick 推进（真机在 SysTick 中断每 1ms 调一次）========== */
void modbus_rx_tick(void)
{
    s_tick++;

    if (s_state == 1 &&              /* 收帧中，且 */
        (s_tick - s_last_tick) >= FRAME_GAP_MS)   /* 距最后字节超时 */
    {
        s_frame_ready = 1;           /* 判定：一帧收齐 */
        s_state = 0;                 /* 回 IDLE 等下一帧 */
    }
}

/* ========== 接口 3：应用层查询 ========== */
uint8_t modbus_rx_is_ready(void)     { return s_frame_ready; }
uint8_t modbus_rx_len(void)          { return s_len; }
uint8_t *modbus_rx_buf(void)         { return s_buf; }
void    modbus_rx_clear(void)        { s_frame_ready = 0; s_len = 0; } /* 处理完清 */

/* ========== 测试 main：模拟"喂帧 + 等超时" ========== */
static void feed_frame(const uint8_t *frame, uint8_t len)  /* 模拟一帧连续到达 */
{
    for (uint8_t i = 0; i < len; i++)
    {
        modbus_rx_byte(frame[i]);    /* 连续喂（模拟一帧字节紧挨着到）*/
    }
}

static void wait_gap(void)           /* 模拟帧间静默：推进超过超时阈值 */
{
    for (int i = 0; i < FRAME_GAP_MS + 5; i++) modbus_rx_tick();
}

int main(void)
{
    /* 测试 1：完整真帧 01 03 00 00 00 02 C4 0B（读 2 寄存器请求）*/
    const uint8_t f1[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
    printf("--- test 1: full valid frame ---\n");
    feed_frame(f1, sizeof(f1));
    wait_gap();                       /* 静默超时 → 帧齐 */
    if (modbus_rx_is_ready())
    {
        printf("frame ready, len=%d: ", modbus_rx_len());
        for (uint8_t i = 0; i < modbus_rx_len(); i++)
            printf("%02X ", modbus_rx_buf()[i]);
        printf("\n");
    }
    modbus_rx_clear();

    /* 测试 2：半帧就停（模拟断帧）→ 超时后也会 ready，但 CRC 层会丢它 */
    printf("--- test 2: half frame then silence ---\n");
    feed_frame(f1, 3);                /* 只喂 3 字节就停 */
    wait_gap();
    if (modbus_rx_is_ready())
        printf("half frame ready, len=%d (CRC layer will drop this)\n", modbus_rx_len());
    modbus_rx_clear();

    /* 测试 3：两帧连发（帧间隔够）→ 应切出两帧 */
    /* ⚠️ ready 是一次性标志：处理完必须立即 clear，否则下一帧的 ready 会被误清 */
    printf("--- test 3: two frames ---\n");
    feed_frame(f1, sizeof(f1)); wait_gap();
    if (modbus_rx_is_ready()) { printf("frame 1 done, len=%d\n", modbus_rx_len()); modbus_rx_clear(); }
    feed_frame(f1, sizeof(f1)); wait_gap();
    if (modbus_rx_is_ready()) { printf("frame 2 done, len=%d\n", modbus_rx_len()); modbus_rx_clear(); }

    printf("--- all tests done ---\n");
    return 0;
}

/**
 * 2026-09-10 UART1 + DMA + IDLE 空闲中断 接收改造参考实现
 * ============================================================
 * 用途：把 09/13 工程 usart.c 的"逐字节中断接收"改造为"DMA 批量接收 + IDLE 判帧尾"
 * 适用：正点原子 MiniSTM32F103 + HAL 库（与 09_stm32_uart 工程同构）
 *
 * 改造总览（对着这张图施工）：
 *   字节流 → [USART1 寄存器] → [DMA1_CH5 自动搬] → g_rx_buffer（卸货区）
 *          → 线上空闲 → [IDLE 中断] → 算帧长 → 交给上层处理 → 重启 DMA
 *
 * 改动清单：
 *   ① usart.h：加 DMA 句柄 extern 声明 + RXBUFFERSIZE 宏确认
 *   ② usart.c：加 g_dma_handle 定义 + usart_rx_dma_init() + 在 usart_init 里调用
 *   ③ usart.c：USART_UX_IRQHandler 加 IDLE 分支
 *   ④ usart.c：HAL_UART_RxCpltCallback 整块退役（注释掉/删除）
 */

/* ============================================================
 * ① usart.c 全局定义区（加在 g_uart1_handle 旁边）
 * ============================================================ */

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/sys/sys.h"


DMA_HandleTypeDef g_dma_handle;              /* DMA 句柄：DMA 也有自己的"配置对象" */

/* RXBUFFERSIZE 建议设成 Modbus 帧上限以上，如 64；USART_REC_LEN 保持原样 */
#define RXBUFFERSIZE    64                    /* DMA 卸货区大小（够装一帧即可）*/


/* ============================================================
 * ② DMA 接收初始化（在 usart_init() 的 HAL_UART_Init() 之后调用；
 *    原来那行 HAL_UART_Receive_IT(...) 删掉，不再要逐字节中断）
 * ============================================================ */
void usart_rx_dma_init(void)
{
    /* ---- 第1步：开 DMA1 时钟 ---- */
    __HAL_RCC_DMA1_CLK_ENABLE();              /* 外设用前先开时钟（和 GPIO/USART 一个道理）*/

    /* ---- 第2步：填 DMA 句柄参数 ---- */
    g_dma_handle.Instance                 = DMA1_Channel5;      /* USART1_RX 固定绑 DMA1 通道5（查手册请求映射表）*/
    g_dma_handle.Init.Direction           = DMA_PERIPH_TO_MEMORY; /* 方向：外设→内存（收数据）*/
    g_dma_handle.Init.PeriphInc           = DMA_PINC_DISABLE;   /* 外设地址不递增：永远读 USART1->DR 这一个寄存器 */
    g_dma_handle.Init.MemInc              = DMA_MINC_ENABLE;    /* 内存地址递增：字节依次放 buf[0]、buf[1]、buf[2]… */
    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;/* 外设数据宽度：串口是字节流，8 位 */
    g_dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE; /* 内存数据宽度：同样 8 位 */
    g_dma_handle.Init.Mode                = DMA_NORMAL;         /* 普通模式：搬满 RXBUFFERSIZE 就停（教学版更好算帧长）*/
    g_dma_handle.Init.Priority            = DMA_PRIORITY_MEDIUM;/* 通道优先级：不关键，中等即可 */
    HAL_DMA_Init(&g_dma_handle);                                /* 生效 */

    /* ---- 第3步：把 DMA 挂到串口句柄上 ---- */
    __HAL_LINKDMA(&g_uart1_handle, hdmarx, g_dma_handle);       /* 让串口知道"我的接收 DMA 是它"（HAL 内部要用）*/

    /* ---- 第4步：启动串口 DMA 接收 ---- */
    HAL_UART_Receive_DMA(&g_uart1_handle, g_rx_buffer, RXBUFFERSIZE);  /* 打开"流水线"：DMA 开始往 g_rx_buffer 搬 */

    /* ---- 第5步：开 IDLE 空闲中断 ---- */
    __HAL_UART_ENABLE_IT(&g_uart1_handle, UART_IT_IDLE);        /* 线上空闲就触发中断（判帧尾的"门铃"）*/
}


/* ============================================================
 * ③ 中断服务函数：加 IDLE 分支（改造 USART_UX_IRQHandler）
 * ============================================================ */
void USART_UX_IRQHandler(void)
{
#if SYS_SUPPORT_OS
    OSIntEnter();
#endif

    /* ---- 自己先查 IDLE 标志（HAL 默认不管空闲中断）---- */
    if (__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&g_uart1_handle);   /* ★必须先清标志，否则中断会反复触发卡死 */

        /* 算这一帧收了多少字节：总长 − DMA 还没搬的计数 */
        uint16_t frame_len = RXBUFFERSIZE - __HAL_DMA_GET_COUNTER(g_uart1_handle.hdmarx);

        if (frame_len > 0)
        {
            /* ★转仓：把这一帧交给上层（下一步接环形缓冲或直接喂 Modbus 状态机）*/
            /* 例：ringbuf_write(g_rx_buffer, frame_len);  或 modbus_rx_feed(g_rx_buffer, frame_len); */
        }

        /* 普通模式下 DMA 已停，重启接收（同时把计数复位，下次帧长才算得对）*/
        HAL_UART_Receive_DMA(&g_uart1_handle, g_rx_buffer, RXBUFFERSIZE);
    }

    HAL_UART_IRQHandler(&g_uart1_handle);   /* 其余中断事件照旧交给 HAL 处理 */

#if SYS_SUPPORT_OS
    OSIntExit();
#endif
}


/* ============================================================
 * ④ 退役：HAL_UART_RxCpltCallback 整块注释掉
 *    （DMA 模式不再逐字节回调；如需保留旧功能可先注释保留）
 * ============================================================ */

/* ============================================================
 * 面试要点（今天搞完后就说得出的四句）
 * 1. 为什么用 DMA：搬运不占 CPU，高速/大数据不丢字节，CPU 只处理不搬运
 * 2. 为什么配 IDLE：DMA 不知道帧长，固定 N 收不了变长帧；线上空闲=帧尾（硬件版"静默判帧"）
 * 3. 帧长怎么算：RXBUFFERSIZE - DMA剩余计数（DMA 从缓冲头搬到当前位置）
 * 4. 为什么循环/普通模式取舍：普通模式+DMA重启=简单可靠；循环模式省重启但要处理回绕计数
 */

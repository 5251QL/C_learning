/**
 * 2026-08-28 UART 中断版 main.c
 *
 * 现象：LED 一直闪（主循环自由跑，不依赖串口）
 *      串口助手发 "1"+回车 → LED 亮 + 回显 ON；发 "0"+回车 → LED 灭 + 回显 OFF
 *
 * 原理：BSP 的 usart.c 已实现中断接收 ——
 *      每收 1 字节 → 硬件打断 → HAL_UART_RxCpltCallback 存字节到 g_usart_rx_buf
 *      收到回车(\r)换行(\n) → 置 g_usart_rx_sta 的 bit15 (0x8000) = 帧完成
 *      主循环只需"瞄一眼"标志位，不用死等
 *
 * 用法：覆盖 09_stm32_uart\User\main.c 后编译烧录
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);     /* BSP 内部已启动中断接收（Receive_IT），不用管 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef cfg = {0};
    cfg.Pin = GPIO_PIN_8;
    cfg.Mode = GPIO_MODE_OUTPUT_PP;
    cfg.Pull = GPIO_PULLUP;
    cfg.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &cfg);

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);  /* LED 自由闪 —— 不被串口卡 */
        delay_ms(200);

        if (g_usart_rx_sta & 0x8000)            /* 中断代收的帧到了？（0x8000=完成） */
        {
            if (g_usart_rx_buf[0] == '1')       /* 帧首字符判断命令 */
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)"ON\r\n", 4, 1000);
            }
            else if (g_usart_rx_buf[0] == '0')
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
                HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)"OFF\r\n", 5, 1000);
            }

            g_usart_rx_sta = 0;                 /* 清标志：帧已消费 */
        }
    }
}

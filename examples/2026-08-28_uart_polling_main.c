/**
 * 2026-08-28 UART 轮询版 main.c（基于用户 8.04 默写代码 + LED 演示）
 *
 * 现象：串口助手不发数据 → LED 完全不闪（主循环被 HAL_UART_Receive 堵死）
 *      发 '1' → LED 亮 + 回显 ON；发 '0' → LED 灭 + 回显 OFF
 *
 * 用法：覆盖 09_stm32_uart\User\main.c 后编译烧录
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

int main(void)
{
    uint8_t rx_byte = 0;

    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);                 /* 注意：BSP 内部其实已启动中断接收 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef cfg = {0};
    cfg.Pin = GPIO_PIN_8;
    cfg.Mode = GPIO_MODE_OUTPUT_PP;
    cfg.Pull = GPIO_PULLUP;
    cfg.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &cfg);

    HAL_UART_AbortReceive_IT(&g_uart1_handle);  /* 停掉 BSP 中断接收，改用轮询 */

    while (1)
    {
        /* ⚠️ 阻塞点：主程序卡在这里死等，最多 5 秒超时 */
        HAL_UART_Receive(&g_uart1_handle, &rx_byte, 1, 5000);

        if (rx_byte == '1')
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);   /* 亮 */
            HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)"ON\r\n", 4, 1000);
        }
        else if (rx_byte == '0')
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);     /* 灭 */
            HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)"OFF\r\n", 5, 1000);
        }

        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);  /* LED 翻转：观察它被卡多久 */
        delay_ms(200);
    }
}

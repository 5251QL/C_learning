#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

int main(void)
{
    uint8_t rx_byte;
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);

    /* ?? BSP,??? PA8 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef cfg = {0};
    cfg.Pin   = GPIO_PIN_8;
    cfg.Mode  = GPIO_MODE_OUTPUT_PP;
    cfg.Pull  = GPIO_PULLUP;
    cfg.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &cfg);

    HAL_UART_AbortReceive_IT(&g_uart1_handle);

    while (1)
    {
        HAL_UART_Receive(&g_uart1_handle, &rx_byte, 1, 5000);
        if (rx_byte == '1')
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
            HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)"ON\r\n", 4, 1000);
        }
        else if (rx_byte == '0')
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
            HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)"OFF\r\n", 5, 1000);
        }
    }
}
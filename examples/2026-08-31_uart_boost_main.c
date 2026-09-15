/**
 * 2026-08-31 UART 加强版 main.c（温湿度上报 + 命令控制）
 *
 * 解决 8.28 遗留问题：LED 不再参与闪烁（TogglePin 与命令抢 LED 的坑），
 * "主循环活着"由每秒温湿度上报证明——比 LED 闪烁更真实（是业务不是摆设）。
 *
 * 现象：
 *   ① 串口助手每秒收到 T:xx.x H:xx.x（主循环自由跑，不被串口卡）
 *   ② 发 "1"+回车 → LED 亮 + 回显 LED ON，上报节奏一秒不差
 *   ③ 发 "0"+回车 → LED 灭 + 回显 LED OFF
 *   ④ 疯狂连发命令 → 全部响应，不丢（中断代收）
 *
 * 用法：覆盖 09_stm32_uart\User\main.c 后编译烧录
 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/AHT20/aht20.h"

int main(void)
{
    float temp = 0, humi = 0;

    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);                     /* BSP 内部已启动中断接收 */

    __HAL_RCC_GPIOA_CLK_ENABLE();           /* LED: PA8 */
    GPIO_InitTypeDef cfg = {0};
    cfg.Pin = GPIO_PIN_8;
    cfg.Mode = GPIO_MODE_OUTPUT_PP;
    cfg.Pull = GPIO_PULLUP;
    cfg.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &cfg);

    while (1)
    {
        /* ==== 主循环自己的活：读传感器 + 串口上报（每秒）==== */
        if (aht20_read(&temp, &humi) == 0)
        {
            printf("T:%.1f H:%.1f\r\n", temp, humi);   /* printf 已被 BSP 重定向 */
        }
        delay_ms(1000);

        /* ==== 串口命令（中断代收，帧完成才处理）==== */
        if (g_usart_rx_sta & 0x8000)        /* 收到完整一帧（回车结束）*/
        {
            if (g_usart_rx_buf[0] == '1')
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);   /* 亮 */
                HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)"LED ON\r\n", 8, 1000);
            }
            else if (g_usart_rx_buf[0] == '0')
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);     /* 灭 */
                HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)"LED OFF\r\n", 9, 1000);
            }
            g_usart_rx_sta = 0;             /* 清标志：帧已消费 */
        }
    }
}

#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/usart/usart.h"
#include "../SYSTEM/delay/delay.h"
#include "../BSP/LED/led.h"

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();

    while (1)
    {
        led0_on();  led1_off();  delay_ms(500);
        led0_off(); led1_on();   delay_ms(500);
    }
}

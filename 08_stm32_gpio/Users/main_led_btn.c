#include "./BSP/LED/led.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"

int main(void)
{
    uint8_t direction = 0;

    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    key_init();

    while (1)
    {
        if (key0_is_pressed())
            direction = !direction;

        if (direction == 0)
            led_chase(150);
        else
            for (int i = LED_NUM - 1; i >= 0; i--)
            {
                led_all_off();
                HAL_GPIO_WritePin(led_table[i].port,
                                  led_table[i].pin,
                                  GPIO_PIN_RESET);
                delay_ms(150);
            }
    }
}
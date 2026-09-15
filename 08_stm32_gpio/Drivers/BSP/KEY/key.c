#include "key.h"

static uint8_t last_stable = 0;   /* 上次稳定后的状态，static 防止外部篡改 */

void key_init(void)
{
    GPIO_InitTypeDef cfg = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    cfg.Pin   = KEY0_PIN;
    cfg.Mode  = GPIO_MODE_INPUT;
    cfg.Pull  = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_PORT, &cfg);
}

/*
 * 返回 1 = 刚松开（下降沿），0 = 无事件
 *
 * 消抖 + 下降沿整合：
 *   ① 读到低电平 → 等 30ms → 再读还是低 → 确认按下
 *   ② last_stable 记录上一次稳定状态
 *   ③ 上次=按下(1)、本次=松开(0) → 返回 1（一次按键事件）
 */
uint8_t key0_is_pressed(void)
{
    uint8_t raw = (HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == GPIO_PIN_RESET) ? 1 : 0;

    /* 消抖：仅当两次采样一致时才信任 */
    if (raw == 1)
    {
        delay_ms(30);
        if (HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == GPIO_PIN_RESET)
        {
            last_stable = 1;   /* 确认按下 */
            return 0;          /* 不是"松手事件" */
        }
    }

    /* 上次稳定=按下 且 本次=松开 → 下降沿 */
    if (last_stable == 1 && raw == 0)
    {
        last_stable = 0;
        return 1;              /* 返回一次松手事件 */
    }

    last_stable = raw;
    return 0;
}
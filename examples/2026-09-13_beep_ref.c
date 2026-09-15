/* ============================================================================
 * 2026-09-13  BEEP BSP —— 参考实现（无源蜂鸣器 / 硬件 PWM 驱动）
 * ----------------------------------------------------------------------------
 * 引脚：PB8 = TIM4_CH3（芯片内部固定映射，不能随便换别的脚）
 * 参数：beep_init(369, 71)
 *         → 频率 = 72MHz / (71+1) / (369+1) ≈ 2703Hz ≈ 2.7kHz
 *         → 占空比 = Pulse/ARR = 185/370 = 50%
 *
 * ⚠️ 三档表述：本文件是 Reasonix 给的【参考版】（9.13 晚用户卡住时的参照物）
 *    用户"独立完成" = 明天白天关掉本文件、从空文件重写一遍，才算数
 * ==========================================================================*/

#include "beep.h"

static TIM_HandleTypeDef  s_beep_handle;      /* 定时器句柄（本文件私有） */
static TIM_OC_InitTypeDef s_beep_oc_handle;   /* 输出比较配置（本文件私有） */

/**
 * @brief  蜂鸣器初始化（配 GPIO + 定时器 PWM 通道）
 * @param  arr : 自动重装值 → 决定【频率】
 * @param  psc : 预分频值   → 决定【计数时钟】
 * @retval 无
 * @note   例：beep_init(369, 71) ≈ 2.7kHz、占空比 50%
 */
void beep_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;

    /* ---- ① 时钟使能：GPIO + 定时器 ---- */
    BEEP_GPIO_CLK_ENABLE();
    BEEP_TIM_CLK_ENABLE();

    /* ---- ② GPIO：复用推挽（把引脚交给定时器控制） ---- */
    gpio_init_struct.Pin   = BEEP_GPIO_PIN;
    gpio_init_struct.Mode  = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BEEP_GPIO_PORT, &gpio_init_struct);

    /* ---- ③ 定时器基础配置 ---- */
    s_beep_handle.Instance           = BEEP_TIM;
    s_beep_handle.Init.Prescaler     = psc;
    s_beep_handle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    s_beep_handle.Init.Period        = arr;
    s_beep_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&s_beep_handle);

    /* ---- ④ 通道配置：PWM1 模式 + 50% 占空比 ---- */
    s_beep_oc_handle.OCMode     = TIM_OCMODE_PWM1;
    s_beep_oc_handle.Pulse      = arr / 2;             /* CCR → 占空比 */
    s_beep_oc_handle.OCPolarity = TIM_OCPOLARITY_LOW;  /* 模块低电平有效；声音异常可改 HIGH */
    HAL_TIM_PWM_ConfigChannel(&s_beep_handle, &s_beep_oc_handle, BEEP_TIM_CHY);

    /* 注意：这里【不】调用 Start —— 什么时候响由 beep_on() 决定 */
}

/**
 * @brief  蜂鸣器开始响
 */
void beep_on(void)
{
    HAL_TIM_PWM_Start(&s_beep_handle, BEEP_TIM_CHY);
}

/**
 * @brief  蜂鸣器停止
 */
void beep_off(void)
{
    HAL_TIM_PWM_Stop(&s_beep_handle, BEEP_TIM_CHY);
}

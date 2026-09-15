#include "stm32f1xx_hal.h"

TIM_HandleTypeDef htim2={0};

void timer_init(void){
    __HAL_RCC_TIM2_CLK_ENABLE();
    htim2.Instance=TIM2;
    htim2.Init.Prescaler=7200-1;
    htim2.Init.Period=10000-1;
    htim2.Init.CounterMode=TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&htim2);
}
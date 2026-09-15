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

int main(){
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    timer_init();

    HAL_TIM_Base_Start_IT(&htim2);

    while(1){

    }
}

void HAL_TIM__PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM2){
        HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
    }
}
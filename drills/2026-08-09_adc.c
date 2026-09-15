#include "stm32f1xx_hal.h"

ADC_HandleTypeDef hadc1={0};

void adc_init(void){
    GPIO_InitTypeDef cfg={0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    cfg.Pin=GPIO_PIN_0;
    cfg.Mode=GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA,&cfg);

    __HAL_RCC_ADC1_CLK_ENABLE();
    hadc1.Instance=ADC1;
    HAL_ADC_Init(&hadc1);
}

int main(){
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);
    adc_init();

    char buf[20];
    while(1){
        uint16_t val=HAL_ADC_GetValue(&hadc1);
        sprintf(buf,"ADC:&d\r\n",val);
        HAL_UART_Transmit(&g_uart1_handle,(uint8_t *)buf,strlen(buf),100);
        HAL_Delay(100);
    }
}
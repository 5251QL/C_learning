#include "stm32f1xx_hal.h"

void led_init(void){
    GPIO_InitTypeDef cfg={0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    cfg.Mode=GPIO_MODE_OUTPUT_PP;
    cfg.Pull=GPIO_PULLUP;
    cfg.Speed=GPIO_SPEED_FREQ_HIGH;
    cfg.Pin=LED0_PIN;
    HAL_GPIO_Init(LED0_PORT,&cfg);
    led_all_off();
}

int main(){
    HAL_UART_Receive(&g_uart_handle,&rx_byte,1,5000);
    while (1){
        if(rx_byte=='1'){
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
            HAL_UART_Transmit(&g_uart_handle,(uint8_t *)"ON\r\n",4,1000);
        }
        else if(rx_byte=='0'){
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
            HAL_UART_Transmit(&g_uart_handle,(uint8_t *)"OFF\r\n",5,1000);
        }
    }
}


I2C_HandleTypeDef hi2c1={0};

void i2c_init(void){
    GPIO_InitTypeDef cfg={0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    cfg.Mode=GPIO_MODE_AF_OD;
    cfg.Speed=GPIO_SPEED_FREQ_HIGH;
    cfg.Pull=GPIO_PULLUP;
    cfg.Pin=GPIO_PIN_6|GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB,&cfg);

    __HAL_RCC_I2c1_CLK_ENABLE();
    hi2c1.Instance=I2C1;
    hi2c1.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.ClockSpeed=100000;
    HAL_I2C_Init(&hi2c1);
}


SPI_HandleTypeDef hspi1={0};

void SPI_init(void){
    GPIO_InitTypeDef gpioa_cfg={0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    gpioa_cfg.Mode=GPIO_MODE_AF_PP;
    gpioa_cfg.Speed=GPIO_SPEED_FREQ_HIGH;
    gpioa_cfg.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA,&gpioa_cfg);

    GPIO_InitTypeDef gpiob_cfg={0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpiob_cfg.Mode=GPIO_MODE_OUTPUT_PP;
    gpiob_cfg.Speed=GPIO_SPEED_FREQ_HIGH;
    gpiob_cfg.Pull=GPIO_PULLUP;
    gpiob_cfg.Pin=GPIO_PIN_0|GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB,&gpiob_cfg);

    __HAL_RCC_SPI1_CLK_ENABLE();
    hspi1.Instance=SPI1;
    hspi1.Init.Mode=SPI_MODE_MASTER;
    hspi1.Init.Direction=SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize=SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPhase=SPI_PHASE_1EDGE;
    hspi1.Init.CLKPolarity=SPI_POLARITY_LOW;
    hspi1.Init.NSS=SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;
    HAL_SPI_Init(&hspi1);
}


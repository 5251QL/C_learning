#include "stm32f1xx_hal.h"

I2C_HandleTypeDef hi2c1={0};


void i2c_init(void){
    GPIO_InitTypeDef gpio_cfg={0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpio_cfg.Pin=GPIO_PIN_6|GPIO_PIN_7;
    gpio_cfg.Mode=GPIO_MODE_AF_OD;
    gpio_cfg.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&gpio_cfg);

    __HAL_RCC_I2C1_CLK_ENABLE();
    hi2c1.Instance=I2C1;
    hi2c1.Init.ClockSpeed=100000;
    hi2c1.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
    HAL_I2C_Init(&hi2c1);
}

uint8_t aht20_read_temp(void){
    uint8_t cmd[3]={0xAC,0x33,0x00};
    uint8_t data[6]={0};

    HAL_I2C_Master_Transmit(&hi2c1,0x70,cmd,3,100);
    HAL_Delay(80);

    HAL_I2C_Master_Receive(&hi2c1,0x71,data,6,100);

    uint32_t raw=((uint32_t)data[3]<<12)
                |((uint32_t)data[4]<<4)
                |((uint32_t)data[5]>>4);
    uint32_t temp=(raw*200*10)/1048576-500;
    return (uint8_t)(temp/10);
}

#include "stm32f1xx_hal.h"

SPI_HandleTypeDef hspi1 = {0};

#define OLED_CS_PIN    GPIO_PIN_0
#define OLED_CS_PORT   GPIOB
#define OLED_DC_PIN    GPIO_PIN_1     // PB1 = 数据/命令选择
#define OLED_DC_PORT   GPIOB

#define OLED_CS_LOW()  HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_RESET)
#define OLED_CS_HIGH() HAL_GPIO_WritePin(OLED_CS_PORT, OLED_CS_PIN, GPIO_PIN_SET)
#define OLED_DC_CMD()  HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_RESET)  // 发命令
#define OLED_DC_DATA() HAL_GPIO_WritePin(OLED_DC_PORT, OLED_DC_PIN, GPIO_PIN_SET)    // 发数据

// 发一个命令字节
void oled_write_cmd(uint8_t cmd) {
    OLED_DC_CMD();                      // DC=低 → "这是指令"
    OLED_CS_LOW();                      // 选中 OLED
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
    OLED_CS_HIGH();                     // 释放
}

// 发一个数据字节
void oled_write_data(uint8_t data) {
    OLED_DC_DATA();                     // DC=高 → "这是数据"
    OLED_CS_LOW();
    HAL_SPI_Transmit(&hspi1, &data, 1, 100);
    OLED_CS_HIGH();
}

void SPI_init(void){
    GPIO_InitTypeDef gpioa_cfg={0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    gpioa_cfg.Mode=GPIO_MODE_AF_PP;
    gpioa_cfg.Pull=GPIO_PULLUP;
    gpioa_cfg.Speed=GPIO_SPEED_FREQ_HIGH;
    gpioa_cfg.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA,&gpioa_cfg);

    GPIO_InitTypeDef gpiob_cfg={0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpiob_cfg.Mode=GPIO_MODE_OUTPUT_PP;
    gpiob_cfg.Pull=GPIO_PULLUP;
    gpiob_cfg.Speed=GPIO_SPEED_FREQ_HIGH;
    gpiob_cfg.Pin=GPIO_PIN_0|GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB,&gpiob_cfg);

    __HAL_RCC_SPI1_CLK_ENABLE();
    hspi1.Instance               = SPI1;             // 选 SPI1 模块
    hspi1.Init.Mode              = SPI_MODE_MASTER;  // MCU 是主设备
    hspi1.Init.Direction         = SPI_DIRECTION_2LINES;  // 双线（收+发）
    hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;     // 每次发 8 位
    hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;      // 时钟空闲时低电平
    hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;       // 第一个边沿采样
    hspi1.Init.NSS               = SPI_NSS_SOFT;          // CS 软件控制
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; // 时钟分频
    HAL_SPI_Init(&hspi1);

}// 步骤 1：GPIO — SPI1 固定引脚：PA5=SCK, PA6=MISO, PA7=MOSI, PB0=C
// 步骤 2：SPI 外设初始化 — 配模式、分频、数据位数
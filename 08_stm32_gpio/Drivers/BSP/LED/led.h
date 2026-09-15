#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

/********************** 配置区：换板子只改这里 **********************/

/*
 * LED_NUM：驱动表条目数，必须和 led_table[] 一致。
 * 增减 LED 步骤：①改宏 ②LED_NUM 改数字 ③ led_table[] 增删行
 */
#define LED_NUM   5

/*
 * 五颗 LED 全在 GPIOB，只开一个端口时钟。
 * 低电平点亮：GPIO 输出 0V → LED 亮。电路：VCC→LED→电阻→GPIO。
 */
#define LED0_PORT       GPIOB
#define LED0_PIN        GPIO_PIN_0

#define LED1_PORT       GPIOB
#define LED1_PIN        GPIO_PIN_1

#define LED2_PORT       GPIOB
#define LED2_PIN        GPIO_PIN_5

#define LED3_PORT       GPIOB
#define LED3_PIN        GPIO_PIN_6

#define LED4_PORT       GPIOB
#define LED4_PIN        GPIO_PIN_7

/********************** API 声明：main.c 调这些 **********************/

void led_init(void);        /* 初始化 GPIO → 全灭                     */
void led_all_off(void);     /* 一键全灭，给跑马灯清场用               */
void led_chase(uint16_t ms);/* 流水灯：逐个点亮，间隔 ms 毫秒         */

/* 便车单灯控制：led0_on() = PB0 输出 0V → 对应的 LED 亮            */
void led0_on(void);
void led0_off(void);
void led1_on(void);
void led1_off(void);
void led2_on(void);
void led2_off(void);
void led3_on(void);
void led3_off(void);
void led4_on(void);
void led4_off(void);

/* 暴露驱动表给 main.c（用于自定义遍历方向） */
typedef struct {
    GPIO_TypeDef *port;
    uint16_t      pin;
} led_entry_t;

extern const led_entry_t led_table[LED_NUM];
 

#endif

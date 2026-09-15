#ifdef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

#define LED_NUM_5

#define LED_PORT    GPIOB
#define LED_PIN     GPIO_PIN_0

#define LED_PORT    GPIOB
#define LED_PIN     GPIO_PIN_1

#define LED_PORT    GPIOB
#define LED_PIN     GPIO_PIN_5

#define LED_PORT    GPIOB
#define LED_PIN     GPIO_PIN_6

#define LED_PORT    GPIOB
#define LED_PIN     GPIO_PIN_7

void led_init(voide);
void led_all_off(void);
void led_chase(unit16_t ms);

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

TypeDef struct{
    GPIO_TypeDef *port;
    unit16_t     pin;
}led_entry_t;

extern const led_entry_t led_table[LED_NUM];

#endif;


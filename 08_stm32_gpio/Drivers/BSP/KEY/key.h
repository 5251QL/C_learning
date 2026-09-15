#ifndef __KEY_H
#define __KEY_H

#include "stm32f1xx_hal.h"

/* 按键引脚 — 正点原子 Mini V4 */
#define KEY0_PORT       GPIOC
#define KEY0_PIN        GPIO_PIN_5

/* 返回值 */
#define KEY_PRESSED     1
#define KEY_RELEASED    0

void key_init(void);
uint8_t key0_is_pressed(void);

#endif


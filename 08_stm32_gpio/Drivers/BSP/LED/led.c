#include "led.h"

/********************** 驱动表：用数据代替代码 *********************/
/*
 * static：数组只在本文件可见，不污染全局命名空间。
 * const：存在 Flash 里，不占 RAM，且不可被意外改写。
 *
 * 作用：led_all_off() 和 led_chase() 不写死引脚名，只遍历这张表。
 *       加新 LED → 只加一行，不改驱动逻辑 —— 表驱动模式。
 */
/* 替换原有的匿名 struct */
const led_entry_t led_table[LED_NUM] = {
    {LED0_PORT, LED0_PIN},
    {LED1_PORT, LED1_PIN},
    {LED2_PORT, LED2_PIN},
    {LED3_PORT, LED3_PIN},
    {LED4_PORT, LED4_PIN},
};

/********************** GPIO 初始化 *********************/

void led_init(void)
{
    /*
     * {0}：整个结构体清零。不这样做的话未显式赋值的字段是垃圾值，
     *       传给 HAL_GPIO_Init 可能导致不可预期的引脚行为。
     */
    GPIO_InitTypeDef cfg = {0};

    /*
     * STM32 为省电默认关闭所有外设时钟。
     * __HAL_RCC_GPIOB_CLK_ENABLE 展开后操作 RCC->APB2ENR 的 IOPBEN 位。
     * 五颗 LED 都在 GPIOB，开一次即可。
     */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*
     * 公共配置：除 Pin 外五颗 LED 完全相同。
     *
     * GPIO_MODE_OUTPUT_PP ：推挽输出，主动输出高低电平，点亮 LED 的标准模式。
     * GPIO_PULLUP         ：默认上拉到 3.3V。LED 一端接 VCC，一端接 GPIO，
     *                       GPIO 输出高电平时两端无压差 → LED 灭，这是安全初始态。
     * GPIO_SPEED_FREQ_HIGH：引脚翻转速度选最高，跑马灯切换无延迟。
     */
    cfg.Mode  = GPIO_MODE_OUTPUT_PP;
    cfg.Pull  = GPIO_PULLUP;
    cfg.Speed = GPIO_SPEED_FREQ_HIGH;

    /*
     * 逐个调用 HAL_GPIO_Init，每次只改 Pin 字段。
     * 也可以一次性用 Pin = PIN_0 | PIN_1 | ... 批量配置，
     * 这里分开写更清晰：LED 序号和 HAL_GPIO_Init 调用一一对应。
     */
    cfg.Pin = LED0_PIN;  HAL_GPIO_Init(LED0_PORT, &cfg);
    cfg.Pin = LED1_PIN;  HAL_GPIO_Init(LED1_PORT, &cfg);
    cfg.Pin = LED2_PIN;  HAL_GPIO_Init(LED2_PORT, &cfg);
    cfg.Pin = LED3_PIN;  HAL_GPIO_Init(LED3_PORT, &cfg);
    cfg.Pin = LED4_PIN;  HAL_GPIO_Init(LED4_PORT, &cfg);

    led_all_off();   /* 初始化完成 → 全灭，等待主循环调度 */
}

/********************** 单灯控制 *********************/

/*
 * GPIO_PIN_RESET = 寄存器写入 0 = GPIO 输出 0V = LED 亮
 * GPIO_PIN_SET   = 寄存器写入 1 = GPIO 输出 3.3V = LED 灭
 *
 * 这些函数仅一行调用，编译器会内联展开，无函数调用开销。
 */
void led0_on(void)  { HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_RESET); }
void led0_off(void) { HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_SET);   }
void led1_on(void)  { HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET); }
void led1_off(void) { HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET);   }
void led2_on(void)  { HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET); }
void led2_off(void) { HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);   }
void led3_on(void)  { HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, GPIO_PIN_RESET); }
void led3_off(void) { HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, GPIO_PIN_SET);   }
void led4_on(void)  { HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, GPIO_PIN_RESET); }
void led4_off(void) { HAL_GPIO_WritePin(LED4_PORT, LED4_PIN, GPIO_PIN_SET);   }

/********************** 跑马灯 *********************/

/*
 * 遍历驱动表，每颗 LED 输出高电平 → 全灭。
 * for (int i...) 是 C99 语法，i 的作用域只在这个 for 循环内。
 */
void led_all_off(void)
{
    for (int i = 0; i < LED_NUM; i++)
        HAL_GPIO_WritePin(led_table[i].port, led_table[i].pin, GPIO_PIN_SET);
}

/*
 * 流水灯核心：
 *   for (i = 0 → N-1)：
 *       ① 全灭      ← 保证之前亮的 LED 先灭
 *       ② 点亮第 i 颗 ← 只有一颗亮，不是堆积点亮
 *       ③ 延时 ms   ← 人眼可见的停留时间
 *
 * uint16_t ms：最大 65535ms，跑马灯场景完全够用。
 */
void led_chase(uint16_t ms)
{
    for (int i = 0; i < LED_NUM; i++)
    {
        led_all_off();
        HAL_GPIO_WritePin(led_table[i].port,
                          led_table[i].pin,
                          GPIO_PIN_RESET);
        delay_ms(ms);
    }
}

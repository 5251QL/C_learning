/*
 * #include 路径设计：
 *   ./ 开头 → 相对于当前文件所在目录（User/）。
 *   Keil 的 Include Paths 提供了查找起点，编译器逐个尝试。
 *   最终解析到正确位置。
 */
#include "./SYSTEM/sys/sys.h"      /* sys_stm32_clock_init：配时钟 */
#include "./SYSTEM/usart/usart.h"  /* 串口（本例未用，留作扩展）    */
#include "./SYSTEM/delay/delay.h"  /* delay_ms：毫秒延时           */
#include "./BSP/LED/led.h"         /* 你的 LED 驱动                */

int main(void)
{
    /* ====== 初始化三步曲（顺序不可颠倒）====== */

    /*
     * 第 1 步：HAL_Init()
     *   - 配置 SysTick 产生 1ms 中断（HAL 时基依赖它）
     *   - 配置 NVIC 中断优先级分组
     *   - 此时 HAL_Delay 尚不可用：时钟未配置，tick 频率不准确
     */
    HAL_Init();

    /*
     * 第 2 步：sys_stm32_clock_init(RCC_PLL_MUL9)
     *   时钟链：HSE(8MHz 外接晶振) → PLL ×9 = 72MHz
     *   设置 AHB=72MHz, APB1=36MHz, APB2=72MHz
     *   此后 SysTick 的时基才校准，HAL_Delay 可用
     */
    sys_stm32_clock_init(RCC_PLL_MUL9);

    /*
     * 第 3 步：delay_init(72)
     *   确认系统时钟是 72MHz，据此计算 SysTick 的重装载值。
     *   至此 delay_ms(n) 的误差在微秒级。
     */
    delay_init(72);

    /*
     * BSP 初始化：开 GPIOB 时钟 → 配置 PB0/1/5/6/7 → 全灭。
     * 换引脚只改 led.h 里的宏，main.c 这行不动。
     */
    led_init();

    /* ====== 主循环：嵌入式程序的心跳 ====== */

    /*
     * while(1) 永不退出。
     * MCU 没有操作系统，运行完 main 后如果没有死循环，
     * PC 寄存器会滑入未定义地址 → HardFault。
     */
    while (1)
    {
        led_chase(200);   /* 5 颗 LED 逐个亮，间隔 200ms，循环往复 */
    }
}

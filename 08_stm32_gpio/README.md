# 08 — STM32 GPIO 点亮 LED（HAL 库 · 分层架构）

## 硬件
- 正点原子 MiniSTM32F103 V4（STM32F103RCT6）
- LED0: PA8（低电平亮）/ LED1: PD2（低电平亮）

## 文件结构
```
08_stm32_gpio/
├── BSP/LED/led.c        ← 板级驱动：led_init/led0_on/led0_off
├── BSP/LED/led.h        ← 引脚宏 + 函数声明
├── User/main.c          ← 调度层：初始化 → while 交替闪烁
├── Drivers/             ← HAL 库 + CMSIS + SYSTEM（不传 GitHub）
└── MDK-ARM/             ← Keil 工程文件
```

## 关键知识点
- GPIO 推挽输出 + 上拉配置
- HAL_GPIO_Init / HAL_GPIO_WritePin
- BSP 分层：main.c 不碰寄存器，led.h 宏定义引脚
- 低电平点亮（LED 阳极接 VCC，阴极接 GPIO）

## 编译 & 烧录
- Keil MDK-ARM v5 + ARM Compiler 5
- ST-Link SWD 模式

## 效果
LED0(红) 和 LED1(绿) 以 500ms 间隔交替闪烁

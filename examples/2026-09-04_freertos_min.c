/*
 * 2026-09-04 FreeRTOS 最小两任务+队列参考（逐行讲解用）
 * 结构：1 个队列 + 2 个任务（生产者/消费者）
 * 编译环境：Keil 13_stm32_project 工程（本文件是核心骨架，非完整工程）
 * 教学顺序：逐行分析 -> 手抄 -> 讲解 -> 独立编写（勿跳步）
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

/* ========== 全局区 ========== */
QueueHandle_t g_queue;   /* 队列句柄：xQueueCreate 返回的"身份证"，
                            所有任务共用这个全局变量来找到队列 */

/* ========== 任务 1：生产者（模拟传感器采集）========== */
void task_sensor(void *pvParameters)   /* 任务函数签名固定：void + 1个 void* 参数 */
{
    int value = 0;                     /* 任务自己的局部变量（每次进函数重新创建）*/

    while (1)                          /* 任务 = 永不退出的循环 */
    {
        value++;
        xQueueSend(g_queue, &value, 0);   /* 发队列：①哪个队列 ②发什么(取地址) ③满
                                             了等多久(0=不等待立刻返回) */
        vTaskDelay(1000);              /* 睡 1000 tick = 1 秒（本工程 1 tick=1ms），
                                          睡眠期间让出 CPU 给别的任务 */
    }
}

/* ========== 任务 2：消费者（模拟显示）========== */
void task_display(void *pvParameters)
{
    int received = 0;                  /* 接收缓冲区：xQueueReceive 把数据拷贝到这里 */

    while (1)
    {
        xQueueReceive(g_queue, &received, portMAX_DELAY);
        /* 收队列：①哪个队列 ②收进哪(取地址) ③空队列等多久(portMAX_DELAY=无限等)
           阻塞 = 挂起睡觉，队列有数据时自动醒来（门铃机制） */
        printf("got: %d\r\n", received);
    }
}

/* ========== 入口 ========== */
void demo_start(void)
{
    g_queue = xQueueCreate(4, sizeof(int));
    /* 创建队列：①槽位数(4) ②每槽字节数(sizeof(int)=4)
       槽位大小必须 >= 数据大小（8.28 教训：sizeof(float) 槽位丢湿度） */

    xTaskCreate(task_sensor,           /* ① 任务函数名（函数地址）*/
                "sensor",              /* ② 任务名（调试器显示用字符串）*/
                128,                   /* ③ 任务堆栈大小（单位：字，128 字=512 字节）*/
                NULL,                  /* ④ 传给任务函数的参数（pvParameters）*/
                1,                     /* ⑤ 任务优先级（数字越大越先跑）*/
                NULL);                 /* ⑥ 任务句柄（删除/挂起用；不要可填 NULL）*/

    xTaskCreate(task_display, "display", 128, NULL, 2, NULL);
    /* 第二个任务：display 优先级 2 > sensor 的 1
       -> sensor 每次发完队列，display 都会抢先去收（高优先级抢占） */

    vTaskStartScheduler();             /* 启动调度器：从这里开始由 RTOS 接管，永不返回 */
}

/* ============================================================================
 * 2026-09-13  FreeRTOS 双任务 + 队列 —— 参考框架（零业务依赖）
 * ----------------------------------------------------------------------------
 * 用途：RTOS「独立编写」练习的参照物（先抄后写）
 *       不涉及 Modbus / AHT20 / OLED，只有三个机制：任务创建、队列、阻塞
 *
 * 练习法（用户 9.13 定）：
 *   ① 抄：照这份敲一遍 —— 抄的时候想"为什么这么写"，不要机械敲
 *   ② 关掉本文件，凭记忆独立写一遍（写到 drills\ 下自己的新文件）
 *   ③ 自查 / 交我检查：结构齐不齐、句柄对不对、阻塞与超时用对没有
 *
 * 抄完自测（面试也会问）：
 *   Q1 为什么先 xQueueCreate 再 xTaskCreate？      → 队列要先存在，任务才有东西可用
 *   Q2 producer 超时 0、consumer 用 portMAX_DELAY？→ 生产者不能被卡住；消费者没活就该睡
 *   Q3 起始任务为什么删掉自己？                      → 它使命只有"创建"，留着白占内存和调度
 *   Q4 队列里为什么传结构体而不是指针？               → 传值=拷贝，避免共享内存被别人改（撕裂）
 * ==========================================================================*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdint.h>

/* ============================ 1. 配置区（宏） ============================ */
#define START_PRIO          1
#define START_STK_SIZE      128     /* 单位: word(4字节); 128 = 512 字节 */

#define PRODUCER_PRIO       2
#define PRODUCER_STK_SIZE   128

#define CONSUMER_PRIO       3
#define CONSUMER_STK_SIZE   128

#define QUEUE_LENGTH        4       /* 队列能放几个元素；满了 producer 会丢弃 */

/* 队列里流动的数据：定长结构体（"定长"是队列能工作的前提） */
typedef struct
{
    uint16_t id;                    /* 第几次采样 */
    int16_t  value;                 /* 模拟的采集值 */
} sample_t;

/* ============================ 2. 句柄区 ============================ */
static QueueHandle_t s_queue_h;       /* 队列句柄; static = 本文件私有 */
static TaskHandle_t  s_start_task_h;
static TaskHandle_t  s_producer_h;
static TaskHandle_t  s_consumer_h;

/* ============================ 3. 声明区 ============================ */
void start_task(void *pvParameters);
void producer_task(void *pvParameters);
void consumer_task(void *pvParameters);

/* ============================ 4. 起始任务 ============================ */
/* 它只干一件事：把别人创建出来，然后删掉自己 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();                                   /* 创建期间不让调度器切走 */

    s_queue = xQueueCreate(QUEUE_LENGTH, sizeof(sample_t));  /* ① 先有队列 */

    xTaskCreate((TaskFunction_t)producer_task,               /* ② 再创建任务 */
                (const char *)"producer",
                (uint16_t)PRODUCER_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)PRODUCER_PRIO,
                (TaskHandle_t *)&s_producer_h);

    xTaskCreate((TaskFunction_t)consumer_task,
                (const char *)"consumer",
                (uint16_t)CONSUMER_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)CONSUMER_PRIO,
                (TaskHandle_t *)&s_consumer_h);

    taskEXIT_CRITICAL();

    vTaskDelete(s_start_task_h);                            /* ③ 删自己（要传句柄，不是 NULL） */
}

/* ============================ 5. 生产者任务 ============================ */
void producer_task(void *pvParameters)
{
    sample_t s;                                             /* 局部变量：发送时会被拷贝进队列 */
    uint16_t n = 0;

    while (1)
    {
        s.id    = n++;
        s.value = (int16_t)(n * 10);

        if (xQueueSend(s_queue, &s, 0) != pdPASS)            /* 超时 0 = 不等待：满了立刻失败 */
        {
            printf("[producer] queue full, drop id=%u\r\n", s.id);
        }

        vTaskDelay(1000);                                    /* TICK_RATE=1000 → 1000 tick = 1 秒 */
    }
}

/* ============================ 6. 消费者任务 ============================ */
void consumer_task(void *pvParameters)
{
    sample_t s;

    while (1)
    {
        /* 永久阻塞：队列空 → 任务进阻塞态（不占 CPU）；有数据 → 被发送方唤醒 */
        if (xQueueReceive(s_queue, &s, portMAX_DELAY) == pdPASS)
        {
            printf("[consumer] id=%u value=%d\r\n", s.id, s.value);
        }
    }
}

/* ============================ 7. 入口示意 ============================ */
/* 项目里 main.c 已经做完时钟/串口初始化，这里只示意最后两句的顺序：

    xTaskCreate((TaskFunction_t)start_task, (const char *)"start",
                (uint16_t)START_STK_SIZE, (void *)NULL,
                (UBaseType_t)START_PRIO, (TaskHandle_t *)&s_start_task_h);
    vTaskStartScheduler();      // 调度器启动；此后 main 永不返回
    while (1);                  // 正常跑不到这（只有堆不足导致调度器启动失败才会返回）
 */

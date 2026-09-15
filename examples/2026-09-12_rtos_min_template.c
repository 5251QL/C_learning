/**
 * 2026-09-12 FreeRTOS 最小模板（两任务 + 队列）—— 抄写练习材料
 * ============================================================
 * 用途：drills 抄写练习（记忆 RTOS API 形态）；在 RTOS 工程里可直接用
 * 抄写目标：关参考写出这个模板（3 天验收：看参考抄 → 关参考抄 → 零错一次过）
 *
 * 结构（四块）：
 *   ① 头文件 + 共享设施（队列句柄）
 *   ② 任务配置（宏 + 句柄 + 声明）
 *   ③ 入口（建队列 → 建起始任务 → 启动调度器）
 *   ④ 起始任务（临界区批量建任务 → 删自己）+ 工作任务（while(1)）
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

/* ───── ① 共享设施：队列句柄 ───── */
typedef struct {
    float temp;
    float humi;
} env_t;

QueueHandle_t g_queue;          /* 信箱句柄（任务间共享，必须全局）*/

/* ───── ② 任务配置（每个任务一套：优先级/堆栈/句柄/声明）───── */
#define START_TASK_PRIO  1
#define START_STK_SIZE   128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define TASK1_PRIO       2
#define TASK1_STK_SIZE   128
TaskHandle_t Task1Task_Handler;
void task1(void *pvParameters);

#define TASK2_PRIO       3
#define TASK2_STK_SIZE   128
TaskHandle_t Task2Task_Handler;
void task2(void *pvParameters);

/* ───── ③ 入口：建队列 + 建起始任务 + 启动调度器 ───── */
void rtos_demo(void)
{
    g_queue = xQueueCreate(4, sizeof(env_t));      /* 4 槽位 × 每槽 8 字节 */

    xTaskCreate((TaskFunction_t)start_task,
                (const char*   )"start_task",
                (uint16_t      )START_STK_SIZE,
                (void*         )NULL,               /* 无参数 */
                (UBaseType_t   )START_TASK_PRIO,
                (TaskHandle_t* )&StartTask_Handler);

    vTaskStartScheduler();                          /* ★此函数不返回 */
}

/* ───── ④-a 起始任务：批量建工作任务 → 删自己 ───── */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();                           /* 临界区：保证创建原子 */

    xTaskCreate((TaskFunction_t)task1, "task1",
                TASK1_STK_SIZE, NULL, TASK1_PRIO, &Task1Task_Handler);

    xTaskCreate((TaskFunction_t)task2, "task2",
                TASK2_STK_SIZE, NULL, TASK2_PRIO, &Task2Task_Handler);

    vTaskDelete(StartTask_Handler);                 /* 使命完成，删自己 */
    taskEXIT_CRITICAL();
}

/* ───── ④-b 工作任务：生产者 / 消费者 ───── */
void task1(void *pvParameters)                      /* 生产者：采集 → 发队列 */
{
    env_t e;
    while (1)
    {
        e.temp = 25.1f;                             /* 实际项目：aht20_read(&e.temp,&e.humi) */
        e.humi = 55.8f;
        xQueueSend(g_queue, &e, 0);                 /* ③=0：队列满就丢，不等待 */
        vTaskDelay(1000);                           /* 阻塞 1s（让出 CPU）*/
    }
}

void task2(void *pvParameters)                      /* 消费者：收队列 → 处理 */
{
    env_t e;
    while (1)
    {
        xQueueReceive(g_queue, &e, portMAX_DELAY);  /* ★阻塞等待（没数据就睡） */
        printf("T=%.1f H=%.1f\r\n", e.temp, e.humi);
    }
}

/* ───── 面试三问（抄完自答）─────
 * 1. 为什么 xQueueCreate 的槽位大小必须是 sizeof(env_t)？（写 sizeof(float) 会怎样）
 * 2. xQueueSend 第 3 参数 0 和 portMAX_DELAY 的区别？
 * 3. 为什么 task2 用 portMAX_DELAY 不会浪费 CPU？（阻塞 vs 轮询）
 */

/*
 * 2026-09-04 RTOS 教学模拟器（PC 可运行版）
 * ============================================
 * 为什么存在：RTOS 代码不落硬件看不见输出。本文件用 Windows 线程(pthread)
 * 模拟 FreeRTOS 的核心机制，让你在 PC 上"看见"任务调度和队列收发。
 *
 * ⚠️ 模拟 vs 真实（务必知道差异，防教歪）：
 *   模拟：任务 = 操作系统线程（真并行/真阻塞）
 *   真实：任务 = FreeRTOS 在单 CPU 上分时调度（伪并行）
 *   行为相似（轮流执行/阻塞唤醒/队列收发），机制不同（线程 vs 上下文切换）
 *
 * 编译运行：gcc -Wall -o rtos_sim 2026-09-04_rtos_sim_demo.c -pthread
 * 输出英文（GBK 终端防乱码）
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <pthread.h>

/* ================= 模拟 API（长相尽量贴近 FreeRTOS） ================= */

typedef void (*TaskFn)(void *);

typedef struct {              /* 模拟"任务"：真身是线程 */
    pthread_t thread;
    char name[16];
} TaskHandle_t;

typedef struct {              /* 传给线程入口的参数包（每任务独立一份）*/
    TaskFn fn;
    void *param;
} TaskArg;

typedef struct {              /* 模拟队列：槽数组 + 锁 + 条件变量 */
    void **slots;             /* 槽指针数组 */
    int capacity;             /* 槽位数 */
    int size;                 /* 每槽数据字节数 */
    int count;                /* 当前占用槽数 */
    int head, tail;           /* 环形读写位置 */
    pthread_mutex_t lock;
    pthread_cond_t not_full;  /* "有空位"信号（等发名单）*/
    pthread_cond_t not_empty; /* "有数据"信号（等收名单）*/
} Queue_t;

#define portMAX_DELAY -1      /* 无限等待（模拟）*/

static void *task_entry(void *p)   /* 线程入口：解包后调用真实任务函数 */
{
    TaskArg *a = (TaskArg *)p;
    a->fn(a->param);          /* 调用任务函数（它内部是 while(1)）*/
    free(a);                  /* 任务退出才释放（教学 demo：永不执行）*/
    return NULL;
}

void xTaskCreate(TaskFn fn, const char *name, int stack, void *param,
                 int prio, TaskHandle_t *handle)
{
    /* 模拟：开一个线程当"任务"。prio/stack 模拟器不生效（真实才生效），
       打印出来让你看见参数被传了 */
    printf("[SIM] create task '%s' (prio=%d)\n", name, prio);
    TaskArg *a = malloc(sizeof(TaskArg));
    a->fn = fn;
    a->param = param;
    if (handle) {
        strncpy(handle->name, name, 15);
        pthread_create(&handle->thread, NULL, task_entry, a);
    } else {
        pthread_t t;
        pthread_create(&t, NULL, task_entry, a);   /* 句柄 NULL：线程照开 */
        pthread_detach(t);
    }
}

void vTaskDelay(int ms)       /* 模拟：睡 ms 毫秒 = 让出 CPU */
{
    Sleep(ms);
}

Queue_t *xQueueCreate(int capacity, int size)
{
    Queue_t *q = calloc(1, sizeof(Queue_t));
    q->capacity = capacity;
    q->size = size;
    q->slots = calloc(capacity, sizeof(void *));
    for (int i = 0; i < capacity; i++) q->slots[i] = malloc(size);
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    printf("[SIM] create queue: %d slots x %d bytes\n", capacity, size);
    return q;
}

int xQueueSend(Queue_t *q, const void *item, int wait_ms)  /* 模拟：满则按 wait 等 */
{
    pthread_mutex_lock(&q->lock);
    while (q->count == q->capacity) {      /* 队列满 */
        if (wait_ms == 0) {                /* 参数 0：不等待，立刻失败返回 */
            pthread_mutex_unlock(&q->lock);
            printf("[SIM] send FAIL: queue full, data dropped!\n");
            return -1;                     /* errQUEUE_FULL */
        }
        pthread_cond_wait(&q->not_full, &q->lock);  /* 挂到"等发名单"睡觉 */
    }
    memcpy(q->slots[q->tail], item, q->size);       /* 拷贝进槽（值拷贝！）*/
    q->tail = (q->tail + 1) % q->capacity;
    q->count++;
    pthread_cond_signal(&q->not_empty);             /* 叫醒等收的任务 */
    pthread_mutex_unlock(&q->lock);
    return 0;                              /* pdPASS */
}

int xQueueReceive(Queue_t *q, void *buf, int wait_ms) /* 模拟：空则按 wait 等 */
{
    pthread_mutex_lock(&q->lock);
    while (q->count == 0) {                /* 队列空 */
        if (wait_ms == 0) {
            pthread_mutex_unlock(&q->lock);
            return -1;
        }
        pthread_cond_wait(&q->not_empty, &q->lock); /* 挂到"等收名单"睡觉 */
    }
    memcpy(buf, q->slots[q->head], q->size);        /* 拷贝出槽 */
    q->head = (q->head + 1) % q->capacity;
    q->count--;
    pthread_cond_signal(&q->not_full);              /* 叫醒等发的任务 */
    pthread_mutex_unlock(&q->lock);
    return 0;
}

/* ================= 用户任务代码（和真 FreeRTOS 写法一致） ================= */

Queue_t *g_queue;

void task_sensor(void *pvParameters)      /* 生产者：每秒造一个数 */
{
    int value = 0;
    while (1) {
        value++;
        xQueueSend(g_queue, &value, 0);   /* ③参数0：满了不等待 */
        printf("  [sensor] sent %d\n", value);
        vTaskDelay(1000);                 /* 睡 1 秒 */
    }
}

void task_display(void *pvParameters)     /* 消费者：等数据到了就收 */
{
    int received = 0;
    while (1) {
        xQueueReceive(g_queue, &received, portMAX_DELAY);  /* 空则无限等 */
        printf("  [display] got %d\n", received);
    }
}

/* ================= main：前半生（模拟器版） ================= */

int main(void)
{
    g_queue = xQueueCreate(4, sizeof(int));   /* 4 槽 × int */

    xTaskCreate(task_sensor, "sensor", 128, NULL, 1, NULL);
    xTaskCreate(task_display, "display", 128, NULL, 2, NULL);

    printf("[SIM] scheduler started, main is out of the game...\n");
    Sleep(4500);   /* 模拟 vTaskStartScheduler：main 让位，跑 4.5 秒后退出 */
    printf("[SIM] demo over (main woke up and killed the demo)\n");
    return 0;
}

#include <stdio.h>
#include <stdint.h>

#define RB_SIZE 16

/* ===== 环形缓冲区（你今天练的，原样用）===== */
typedef struct {
    uint8_t buf[RB_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
} rb_t;

void rb_init(rb_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

int rb_write(rb_t *rb, uint8_t data) {
    if (rb->count == RB_SIZE) return 0;
    rb->buf[rb->tail] = data;
    rb->tail = (rb->tail + 1) % RB_SIZE;
    rb->count++;
    return 1;
}

int rb_read(rb_t *rb, uint8_t *out) {
    if (rb->count == 0) return 0;
    *out = rb->buf[rb->head];
    rb->head = (rb->head + 1) % RB_SIZE;
    rb->count--;
    return 1;
}

/* ===== 全局缓冲区：模拟"中断和主循环共享" ===== */
static rb_t uart_rb;

/* ===== 模拟串口中断：收到一个字节就存入缓冲区（生产者）===== */
static void uart_rx_isr(uint8_t byte) {
    if (rb_write(&uart_rb, byte)) {
        printf("[ISR] stored %d (count=%d)\n", byte, uart_rb.count);
    } else {
        printf("[ISR] BUFFER FULL! dropped %d\n", byte);
    }
}
/* ===== 主循环消费：每次取出所有能取的数据（消费者）===== */
static void process_loop(void) {
    uint8_t v;
    while (rb_read(&uart_rb, &v)) {
        printf("[MAIN] got %d\n", v);
    }
}

int main(void) {
    rb_init(&uart_rb);

    printf("== sim: uart rx 4 bytes (isr stores) ==\n");
    uart_rx_isr(10);
    uart_rx_isr(20);
    uart_rx_isr(30);
    uart_rx_isr(40);

    printf("== main loop consumes (drain all) ==\n");
    process_loop();

    printf("== 2 more bytes arrive while main is busy ==\n");
    uart_rx_isr(50);
    uart_rx_isr(60);           /* 数据先存着，主程序忙 */

    printf("== main done, now drain ==\n");
    process_loop();

    printf("== stress test: push 20 bytes (buf=16) ==\n");
    for (int i = 0; i < 20; i++) {
        uart_rx_isr(100 + i);  /* 100~119 共20个，缓冲区16格 */
    }
    printf("== drain all data ==\n");
    process_loop();

    return 0;
}

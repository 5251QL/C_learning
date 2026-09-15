#include <stdio.h>

#define RB_SIZE 4        // 故意设小：4个格子，方便看"绕圈"效果

/* ===== ① 结构体：数据区 + 读写指针 + 数量（这是"封装"）===== */
typedef struct {
    int  buf[RB_SIZE];   // 4个格子的停车场
    int  head;           // 读指针：下一个要读的位置
    int  tail;           // 写指针：下一个要写的位置
    int  count;          // 当前停了几辆车（元素个数）
} rb_t;

/* ===== ② 初始化：三个成员全部清零 ===== */
void rb_init(rb_t *rb) {
    rb->head  = 0;
    rb->tail  = 0;
    rb->count = 0;
}

/* ===== ③ 写一个数据。满 → 返回0失败；成功 → 返回1 ===== */
int rb_write(rb_t *rb, int data) {
    if (rb->count == RB_SIZE) {      // 满了（count就是干这个的）
        printf("[FULL] cannot write %d\n", data);
        return 0;
    }
    rb->buf[rb->tail] = data;                        // 放车
    rb->tail = (rb->tail + 1) % RB_SIZE;             // 写指针前进，到末尾绕回0
    rb->count++;                                     // 数量+1
    return 1;
}

/* ===== ④ 读一个数据。空 → 返回0失败；成功 → 数据通过 *out 带出 ===== */
int rb_read(rb_t *rb, int *out) {
    if (rb->count == 0) {            // 空了
        printf("[EMPTY] nothing to read\n");
        return 0;
    }
    *out = rb->buf[rb->head];                        // 取车
    rb->head = (rb->head + 1) % RB_SIZE;             // 读指针前进，绕回0
    rb->count--;                                     // 数量-1
    return 1;
}

/* ===== ⑤ 打印停车场当前状态（帮你看懂指针怎么转）===== */
void rb_dump(const rb_t *rb) {
    printf("  head=%d tail=%d count=%d\n", rb->head, rb->tail, rb->count);
}

/* ===== ⑥ 测试主函数 ===== */
int main(void) {
    rb_t rb2;
    rb_init(&rb2);        // 传地址！让子函数能改到真正的 rb2
    int v;

    /* --- 第1幕：写4个（正好装满4格） --- */
    printf("== write 10 20 30 40 ==\n");
    rb_write(&rb2, 10);
    rb_write(&rb2, 20);
    rb_write(&rb2, 30);
    rb_write(&rb2, 40);
    rb_dump(&rb2);

    /* --- 第2幕：再写1个 → 应该"满"拒绝 --- */
    rb_write(&rb2, 50);
    rb_dump(&rb2);

    /* --- 第3幕：读2个，腾出2格 --- */
    printf("== read 2 items ==\n");
    if (rb_read(&rb2, &v)) printf("  got %d\n", v);
    if (rb_read(&rb2, &v)) printf("  got %d\n", v);
    rb_dump(&rb2);

    /* --- 第4幕：再写2个 → 数据应该写到"绕回开头"的空位 --- */
    printf("== write 99 88 ==\n");
    rb_write(&rb2, 99);
    rb_write(&rb2, 88);
    rb_dump(&rb2);

    /* --- 第5幕：全部读出 --- */
    printf("== read all ==\n");
    while (rb_read(&rb2, &v)) printf("  got %d\n", v);
    rb_dump(&rb2);

    return 0;
}

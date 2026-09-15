/**
 * 2026-09-02 指针全景代码（带注释版）——今天讲的所有指针内容一览
 *
 * 【内存心智模型】
 *   内存 = 一排编了号的格子（每格 = 1 字节）
 *   变量 = 占几格 + 有名字；指针 = 存"格子地址"的变量
 *
 * 【三句总纲】
 *   ① 声明 int *p;      → p 是一个存地址的格子
 *   ② 解引用 *p         → "p 指向的那个房间"（可读可写）
 *   ③ 先指向，再解引用   → 野指针 = 没指向就解引用
 *   一级指针改变量内容 / 二级指针改指针本身
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>   /* malloc/free 需要 */

/* ========== 1. 基本类型 & 地址 ========== */
void demo_basic(void)
{
    int x = 5;
    printf("x 的值: %d\n", x);
    printf("x 的地址: %p\n", (void *)&x);   /* & = 取地址，地址是"房间号" */
}

/* ========== 2. int* 一级指针：改变量内容 ========== */
void demo_int_pointer(void)
{
    int x = 5;
    int *p = &x;       /* p 拿到 x 的房间号 */
    *p = 100;          /* 解引用：进房间改值 → x 变 100 */
    printf("改后 x = %d\n", x);   /* 100 */
}

/* ========== 3. 一级指针传参（swap）：改调用者的变量 ========== */
void swap_int(int *a, int *b)     /* 拿房间号，才能改房间 */
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ========== 4. char* 字符串：按位置访问 ========== */
void demo_string(void)
{
    char *s = "hello";
    /* 内存:  [0]h [1]e [2]l [3]l [4]o [5]\0 */
    printf("s[0]=%c  s[4]=%c\n", s[0], s[4]);        /* h o */
    printf("strlen(s)=%d\n", (int)strlen(s));        /* 5（不含 \0）*/
    /* 字符串占 6 字节：5 字符 + 1 个 '\0' 哨兵（strlen 数到它就停）*/
}

/* ========== 5. 二级指针：改"指针本身"（无结构体场景） ========== */
void swap_str(char **a, char **b)  /* 想换掉调用者的指针，要传指针的地址 */
{
    char *tmp = *a;
    *a = *b;           /* *a = 改调用者那个指针的"指向" */
    *b = tmp;
}

/* ========== 6. struct* 结构体指针：按名字访问（链表） ========== */
typedef struct Node {
    int data;
    struct Node *next;
} Node;

void demo_struct_pointer(void)
{
    Node n1 = {10, NULL};
    Node *p = &n1;
    p->data = 20;              /* -> = 顺着指针访问成员（按名字！）*/
    printf("n1.data = %d\n", n1.data);   /* 20 */
}

/* ========== 7. 二级指针 + struct：链表"改头"（已会，回顾） ========== */
void append_head(Node **head, int data)  /* 空链表时 *head 要变 → 二级 */
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = *head;    /* 新节点指向旧头（空链表时 *head=NULL，天然对）*/
    *head = new_node;          /* 改调用者的 head */
}

/* ========== 8. 数组名 = 指针 + 下标 = 偏移 ========== */
void demo_array_pointer(void)
{
    char arr[] = "abc";
    char *p = arr;             /* 数组名 = 首元素地址，可直接赋给指针 */
    printf("arr[1]=%c  *(p+1)=%c  相同！\n", arr[1], *(p + 1));   /* b b */
}

int main(void)
{
    printf("=== 1. 基本类型与地址 ===\n");
    demo_basic();

    printf("\n=== 2. int* 一级指针 ===\n");
    demo_int_pointer();

    printf("\n=== 3. swap（一级指针传参）===\n");
    int a = 1, b = 2;
    swap_int(&a, &b);          /* 传地址 → 函数能改 a/b */
    printf("a=%d b=%d\n", a, b);   /* 2 1 */

    printf("\n=== 4. char* 字符串 ===\n");
    demo_string();

    printf("\n=== 5. 二级指针 swap_str（无结构体！）===\n");
    char *x = "hello";
    char *y = "world";
    swap_str(&x, &y);          /* 传指针的地址 → 换"指向" */
    printf("x=%s y=%s\n", x, y);   /* world hello（%s = 打印字符串内容）*/

    printf("\n=== 6. struct* 结构体指针 ===\n");
    demo_struct_pointer();

    printf("\n=== 7. 二级指针 + struct（链表改头）===\n");
    Node *head = NULL;
    append_head(&head, 30);
    append_head(&head, 20);
    append_head(&head, 10);
    for (Node *p = head; p != NULL; p = p->next)
        printf("%d ", p->data);    /* 10 20 30（头插=倒序插入）*/
    printf("\n");

    printf("\n=== 8. 数组名与下标 ===\n");
    demo_array_pointer();

    /* ========== 9. 野指针 vs 正确初始化 ========== */
    printf("\n=== 9. 野指针 ===\n");
    int value = 7;
    int *good = &value;        /* ✅ 先指向 */
    *good = 99;                /* ✅ 再解引用 */
    /* int *bad; *bad = 5;     ❌ 野指针：bad 没指向任何房间就写 → 崩溃/静默破坏 */
    printf("value = %d（good 指针改的）\n", value);

    return 0;
}

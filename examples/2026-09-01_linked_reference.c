/**
 * 2026-09-01 链表修正参考版（明天对照 drills/2026-09-01_linked.c 看）
 * 7 处错误修正，每处都有注释说明"为什么"
 *
 * 核心口诀：
 *   判空永远判"指针"（p != NULL），不判"数据"（p->data）
 *   赋值要写 = 别写 ==（if(p == NULL) 不是 if(p = NULL)）
 *   要"改头"的函数必须传二级指针（Node **head）
 *   函数声明了什么类型，调用和返回就要配什么类型
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* ① 创建节点：malloc + 填数据 + 返回指针 */
Node *create(int data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));

    if (new_node == NULL)           /* 修正1：== 不是 =（=是赋值，会把指针清零）*/
    {
        printf("malloc fail\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;

    return new_node;                /* 修正2：必须 return！函数声明了返回 Node* */
}

/* ② 尾插：二级指针 head = "改头"的钥匙 */
void append(Node **head, int data)  /* 修正4：main 传 &head，形参必须 Node** */
{
    Node *new_node = create(data);

    if (*head == NULL)              /* 空链表：直接改头（*head = 解引用拿到 main 的 head）*/
    {
        *head = new_node;
        return;
    }

    Node *p = *head;
    while (p->next != NULL)         /* 找尾巴：p->next 是 NULL 时 p 就是尾 */
    {
        p = p->next;
    }
    p->next = new_node;             /* 尾的 next 挂新节点 */
}

/* ③ 打印 */
void print(Node *head)
{
    Node *p = head;
    while (p != NULL)               /* 修正3：判指针！p 走到 NULL 才停 */
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

/* ④ 删除指定值：二级指针（删头要改头）+ 找到才 free */
void delete_node(Node **head, int target)
{
    Node *p = *head;
    Node *prev = NULL;

    while (p != NULL && p->data != target)  /* 修正3：p != NULL 在前（短路求值）*/
    {
        prev = p;
        p = p->next;
    }

    if (p == NULL)                  /* 没找到：不删任何东西，直接返回 */
    {
        printf("no target\n");
        return;
    }

    if (prev == NULL)               /* 删的是头：改头（这就是二级指针的意义）*/
    {
        *head = p->next;            /* 修正5：*head 改 main 的 head，不是 prev */
    }
    else                            /* 删中间/尾部：前一个的 next 跳过 p */
    {
        prev->next = p->next;
    }

    free(p);                        /* 修正6：只有找到才 free，且 free 的是 p */
}

/* ⑤ 反转：返回新头（调用方要接收返回值）*/
Node *reverse(Node **head)
{
    Node *prev = NULL;
    Node *curr = *head;
    Node *next = NULL;

    while (curr != NULL)            /* 修正3：判指针 */
    {
        next = curr->next;          /* 先存下一个（否则断链）*/
        curr->next = prev;          /* 当前指向前一个（掉头）*/
        prev = curr;                /* 三指针平移 */
        curr = next;
    }
    return prev;                    /* prev 是新的头 */
}

int main(void)
{
    Node *head = NULL;

    append(&head, 10);
    append(&head, 20);
    append(&head, 30);

    print(head);                    /* 10 20 30 */

    head = reverse(&head);          /* 接收新头 */
    print(head);                    /* 30 20 10 */

    delete_node(&head, 10);         /* 删头场景 */
    delete_node(&head, 40);         /* 找不到场景 */
    print(head);                    /* 30 20 */

    return 0;
}

/*
 * 2026-08-22 链表 reverse 超详细注释版
 * 配套指针心智模型：房间 + 门牌号（纸条）
 *   - 节点 = 房间（有 data 格子和 next 格子）
 *   - head / prev / curr / next = 纸条（装着房间的门牌号）
 *   - p->next = 拿纸条找到房间，动里面的 next 格子
 *   - p = q   = 改纸条本身（换方向）
 *
 * 用 1→2→3 链表演示，每步都标注三个纸条的变化
 */

#include <stdio.h>
#include <stdlib.h>

/* 节点 = 一个房间：一格 data + 一格 next（next 装下一个房间的门牌号） */
struct Node{
    int data;
    struct Node *next;
};

/* create：开一间新房间（malloc 向系统要一块内存），把门牌号交回 */
struct Node *create(int data)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    /* 生产级代码必须判空：malloc 失败会返回 NULL（房间没开成） */
    if (new_node == NULL){
        printf("malloc fail!\n");
        return NULL;          /* 交回"没房间"这个信号 */
    }
    new_node->data = data;    /* 进房间，把 data 放进 data 格子 */
    new_node->next = NULL;    /* next 格子先空着（房间还没连出去） */
    return new_node;          /* 把门牌号交回给调用者 */
}

/*
 * append：尾插（复习二级指针）
 * 形参 struct Node **head：装的是 main 的 head 这张纸条的【门牌号】
 * 为什么要两级？因为空链表时要改 head 这张纸条本身 → 必须拿到它的门牌号
 */
void append(struct Node **head, int data)
{
    struct Node *new_node = create(data);   /* 先开一间新房间 */

    if (*head == NULL){          /* *head = 拿门牌号找到 main 的 head 纸条，看它指向哪 */
        *head = new_node;        /* 把新房间的门牌号写进 main 的 head 纸条 */
        return;
    }
    struct Node *p = *head;      /* 抄一张 main 的 head 纸条的副本给 p，让 p 去走 */
    while (p->next != NULL){     /* 只要当前房间的 next 格子不是空的 */
        p = p->next;             /* p 这张纸条换方向：指向 next 格子里那个房间 */
    }
    p->next = new_node;          /* 走到最后一个房间，把新房间的门牌号写进它的 next 格子 */
}

/* reverse：三指针反转（今天的主角） */
struct Node *reverse(struct Node *head)
{
    struct Node *prev = NULL;    /* 纸条 prev：先空着（还没有"上一个"） */
    struct Node *curr = head;    /* 纸条 curr：先指向头房间 */
    struct Node *next = NULL;    /* 纸条 next：先空着 */

    while (curr != NULL)         /* 只要 curr 还有房间可指 */
    {
        /* 第①步：先救走下一位！
         * curr->next = 拿 curr 纸条找到房间，看它的 next 格子（里面是下一个房间的门牌号）
         * 把这张门牌号抄给 next 纸条 —— 不救走的话，第②步一改就找不回后面了 */
        next = curr->next;

        /* 第②步：箭头掉头！改的是【当前房间的 next 格子】，不是纸条
         * 原来 next 格子指向前方（下一个房间），现在改成指向 prev（上一个房间） */
        curr->next = prev;

        /* 第③步：纸条 prev 前进 —— 改的是【纸条本身】（换方向）
         * prev 从"指向前一个"变成"指向当前房间" */
        prev = curr;

        /* 第④步：纸条 curr 前进 —— 也是改纸条本身
         * curr 从"指向当前房间"变成"指向第①步救下来的 next" */
        curr = next;
    }
    /* 循环结束时 curr = NULL（走完了），prev 停在最后一个房间 —— 它就是新头 */
    return prev;
}

/* print：打印整条链（p 是临时纸条，不污染 head） */
void print(struct Node *head)
{
    struct Node *p = head;       /* 抄一份 head 纸条给 p */
    while (p != NULL){
        printf("%d ", p->data);  /* 进房间读 data */
        p = p->next;             /* p 换方向到下一个房间 */
    }
    printf("\n");
}

int main(void)
{
    struct Node *head = NULL;    /* 纸条 head：初始空（还没链表） */

    append(&head, 1);            /* &head = 拿 head 纸条的门牌号给 append */
    append(&head, 2);            /*     （因为 append 可能要改 head 纸条本身） */
    append(&head, 3);

    printf("original: ");
    print(head);                 /* 1 2 3 */

    head = reverse(head);        /* ★ 必须接返回值！
                                  * reverse 拿到的只是 head 纸条的【副本】
                                  * 它改的是节点房间（箭头掉头），改不到 main 的 head
                                  * 反转后新头是 3 号房间，必须靠 return 交回，
                                  * main 自己把新门牌号写进 head 纸条 */
    printf("reversed: ");
    print(head);                 /* 3 2 1 */

    return 0;
}

// 2026-08-20 链表参考示例：create + append + reverse + print
// 抄写时带着三个问题：
//   ① append 为什么参数是 struct Node **head（两个星号）？
//   ② reverse 为什么要三个指针，少一个行不行？
//   ③ main 里 head = reverse(head) 为什么不接住返回值就会丢链表？

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

// 建一个节点（内存来自堆，用完要 free）
struct Node *create(int data){
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// 尾部追加。为什么是 **head？因为空链表时，要改的是 head 这个变量本身
void append(struct Node **head, int data){
    struct Node *new_node = create(data);
    if(*head == NULL){           // 空链表：新节点直接当头
        *head = new_node;
        return;
    }
    struct Node *p = *head;
    while(p->next != NULL){      // 走到最后一个节点
        p = p->next;
    }
    p->next = new_node;          // 最后一个的 next 挂上新节点
}

// 反转：三指针，逐个把箭头掉头
struct Node *reverse(struct Node *head){
    struct Node *prev = NULL;
    struct Node *curr = head;
    struct Node *next = NULL;
    while(curr != NULL){
        next = curr->next;       // ① 先记住下一个，否则箭头掉头后就找不到了
        curr->next = prev;       // ② 当前箭头掉头，指向前一个
        prev = curr;             // ③ prev 前进
        curr = next;             // ④ curr 前进
    }
    return prev;                 // 循环结束时，prev 就是新链表的头
}

// 打印整条链
void print(struct Node *head){
    struct Node *p = head;
    while(p != NULL){
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

int main(){
    struct Node *head = NULL;    // 初始空链表
    append(&head, 1);
    append(&head, 2);
    append(&head, 3);
    printf("original: ");
    print(head);

    head = reverse(head);        // 不接住返回值 → 链表就丢了
    printf("reversed: ");
    print(head);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

struct Node{
    double data;
    struct Node *next;
};

//创建节点
struct Node *create_node(double data){
    struct Node *node=(struct Node*)malloc(sizeof(struct Node));
    if(node==NULL){
        printf("malloc fail\n");
        exit(1);
    }
    node ->data=data;
    node ->next=NULL;
    return node;
};

//在链表尾部插入
void append(struct Node **head,double data){
    struct Node *new_node=create_node(data);
    if(*head==NULL){
        *head=new_node; // 如果链表是空的，就让头指针直接指向新节点，然后结束
        return;
    }
    //找到最后一个节点
    struct Node *p= * head;
    while(p->next !=NULL){
    p=p->next; // 一直往后走，直到找到最后一个节点（它的 next 为 NULL）
    }
    p->next=new_node;
}
//打印整个链表
void printf_list(struct Node *head){
    struct Node *p=head;
    while(p!=NULL){
        printf("%.1f->",p->data);
        p=p->next;
    }
    printf("NULL\n");
}

int main(){
    struct Node *head=NULL; //空链表，现在指针为空

    append(&head,10.5); //传&head，让append能修改head本身
    append(&head,20.5);// 传 &head，让 append 有能力改 head 本身（空链表时把 head 从 NULL 改成指向新节点）

    append(&head,30.5);

    printf_list(head);

    return 0;
}
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
        *head=new_node;
        return;
    }
    //找到最后一个节点
    struct Node *p= * head;
    while(p->next !=NULL){
    p=p->next;
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

void free_list(struct Node *head){
    struct Node *p=head;
    while (p!=NULL){
        struct Node *next=p->next;
        free(p);
        p=next;
    }
}
int main(){
    struct Node *head=NULL; //空链表，现在指针为空

    append(&head,10.5); //传&head，让append能修改head本身
    append(&head,20.5);
    append(&head,30.5);

    printf_list(head);

    return 0;
}
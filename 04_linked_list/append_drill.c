#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node *create_node(int data){
    struct Node *node=(struct Node *)malloc(sizeof(struct Node));
    if(node==NULL){
        printf("malloc fail");
        exit(1);
    }
    node->data=data;
    node->next=NULL;
    return node;
    
}

//append：在链表尾部插入新节点
//参数head：指向main中的head的指针（二级指针，因为可能需要修改main的head
//参数data：要插入的数据
void append(struct Node **head,int data){
    //1.造一个新节点
    struct Node *new_node=create_node(data);

    //2.如果链表是空的---新节点直接当头
    //*head就是main的head
    //即，main中的head=new_node
    if (*head==NULL){
        *head=new_node;
        return;
    }

    //3.链表不为空---找到最后一个节点
    struct  Node *p=*head; //链表头开始走
    while(p->next!=NULL){ //只要后面还有节点
        p=p->next; //就往前走
    }
    //退出时，p指向最后一个节点

    //4.新节点挂在最后
    p->next=new_node;
}

int main(){
    struct Node *head=NULL;

    append(&head,10); //此时，链表只有一个节点[10]
    append(&head,20); //链表：[10]->[20]
    append(&head,30); //链表：[10]->[20]->[30]

    //遍历打印
    struct Node *p=head;
    while (p!=NULL){
        printf("%d->",p->data);
        p=p->next;
    }
    printf("NULL\n");

    return 0;
}
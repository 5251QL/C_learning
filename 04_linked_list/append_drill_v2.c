#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

//创建节点的“工厂函数”
//参数：data---要存的数据
//返回：指向新节点的指针(struct Node*)
struct Node *create_node(int data){ //借出内存大小刚好等于下一个Node,把地址交给node
    struct Node *node=(struct Node*)malloc(sizeof(struct Node));
    if(node==NULL){ //如果malloc返回NULL值(无内存)，打印错误信息，退出程序
        printf("malloc fail \n");
        exit(1);
    }
    node->data=data; //把传入data的值装进新节点
    node->next=NULL; //没有后续节点，暂时为空
    return node; //把新节点交出去
}

void append(struct Node **head,int data){ //二级指针，append可能修改main中的head地址(从NULL指向新节点)
    //调用create——node创造新节点，存入new_node
    struct Node  *new_node=create_node(data);
    //判断链表是否为空
    //*head是main中的head，如果main的head是NULL，链表为空
    if(*head==NULL){
        *head=new_node; //新节点直接当作链表的头
        return; //空链表处理完毕，结束函数
    }
    //找到最后一个节点
    struct Node *p=*head; //p从链表头开始走
    while(p->next!=NULL){ //只要下一个节点不为NULL，保持继续
        p=p->next; //p直接指向下一个节点
    }
    //退出时，p指向最后一个节点

    //把新节点挂入最后一个节点的末端
    p->next=new_node;
}

int main(){
    struct Node*head=NULL;
    append(&head,10);
    append(&head,20);
    append(&head,30);

    struct Node *p=head; //链表从表头开始
    while(p!=NULL){
        printf("%d->",p->data);
        p=p->next;
    }
    printf("NULL\n");
    return 0;
}

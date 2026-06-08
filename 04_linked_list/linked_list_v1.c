#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

int main(){
    //创建三个节点，每一个都是一块malloc出来的内存
    struct Node *head=(struct Node *)malloc(sizeof(struct Node));
    struct Node *second=(struct Node *)malloc(sizeof(struct Node));
    struct Node *third=(struct Node*)malloc(sizeof(struct Node));

    //装数据
    head->data=10;
    second->data=20;
    third->data=30;
    
    //串起来：head-second-third-NULL
    head->next=second;
    second->next=third;
    third->next=NULL; //表示链表到此结束

    //遍历打印
    struct Node*p=head;
    while(p!=NULL){
        printf("jiedian:%d\n",p->data);
        p= p->next; //指针指向下一个节点
    }

    //释放
    free(head);
    free(second);
    free(third);

    return 0;
}
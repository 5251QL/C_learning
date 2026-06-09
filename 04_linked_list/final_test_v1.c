#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

int main(){
    struct Node *p=(struct Node *)malloc(sizeof(struct Node));
     struct Node *pa=(struct Node *)malloc(sizeof(struct Node));
      struct Node *ps=(struct Node *)malloc(sizeof(struct Node));
    if (p==NULL||pa==NULL||ps==NULL){ //if (p==NULL){,此部分为错误，需注意
        printf("malloc fail");
        return 1; //修改部分：需要退出
    }

    p->data=10;
    pa->data=20;
    ps->data=30;

    p->next=pa;
    pa->next=ps;
    ps->next=NULL;

    //遍历打印
    struct Node *cur=p;
    while(cur!=NULL){
        printf("%d\n",cur->data);
        cur=cur->next; //不可缺少，否则无法实现遍历
    }

    free(p);
    free(pa);
    free(ps);

    return 0;

}
#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node create_node(int data){
    struct Node *p=(struct Node *)malloc(sizeof(struct Node));
    if(p==NULL){
        printf("malloc fail");
        exit(1);
    }
    p->data=data;
    p->next=NULL;
}

struct Node new_node(struct Node **head,int data){
    struct Node *new_node=create_node(data);
    if(head!=NULL){
        head->next;        
    }
    struct Node *p=*head;
    p->next;
}

int main(){
    append(&head,10);
    append(&head,20);
    append(&head,30);

    while(head!=NULL){
        printf("p->%d\n",p->next);
    }

    free()
}






struct Node *p=n1;
while(p!=NULL){
    printf("p->%d\n",p->data);
    p->next;
}
printf("NULL\n");

#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node *create(int data){
    struct Node *n=(struct Node *)malloc(sizeof(struct Node));
    if(n==NULL){
        printf("NULL");
        exit(1);
    }
    n->data=data;
    n->next=NULL;;
    return n;
}

void append(struct Node **head,int data){
    struct Node *new=create(data);

    if(*head==NULL){
        *head=new;
        return;
    }

    struct Node *finger=*head;
    while(finger->next!=NULL){
        finger=finger->next;
    }
    finger->next=new;
}

void print(struct Node *finger){
    while(finger!=NULL){
        printf("%d->",finger->data);
        finger=finger->next;
    }
    printf("NULL\n");
}

int main(){
    struct Node *head=NULL;

    append(&head,10);
    append(&head,20);
    append(&head,30);
    append(&head,40);
    append(&head,50);

    print(head);
}
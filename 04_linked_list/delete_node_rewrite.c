#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node *create_node(int data){
    struct Node *node=(struct Node *)malloc(sizeof(struct Node));
    if(node==NULL){
        printf("malloc fail\n");
    }
    node->data=data;
    node->next=NULL;
    return node;
}

void append(struct Node **head,int data){
    struct Node *new_node=create_node(data);

    if(*head==NULL){
        *head=new_node;
        return;
    }
    
    struct Node *p=*head;
    if(p->next!=NULL){
        p=p->next;
    }
    p->next=new_node;
}

void print_list(struct Node *list){
    struct Node *finger=list;
    while(finger!=NULL){
        printf("%d->",finger->data);
        finger=finger->next;
    }
    printf("NULL\n");
}

void delete_node(struct Node **head,int target){
    struct Node *p=*head;
    struct Node *prve=NULL;

    while(p!=NULL&&p->next!=NULL){
        p=p->next;
    }
    if(p=NULL){
        p=head;

    }
}
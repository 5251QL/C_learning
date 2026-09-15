#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node *create_node(struct Node **head,int data){
    struct Node *new_node=create_node(data);
    if(*head==NULL){
        *head=new_node;
        return;
    }

    struct Node *finger=*head;
    while(finger!=NULL){
        finger=finger->next;
    }
    finger->next=new_node;
}

void print_list(struct Node *list){
    struct Node *finger=list;
    while(finger!=NULL){
        printf("%d ", finger->data);
        finger=finger->next;
    }
    printf("\n");
}


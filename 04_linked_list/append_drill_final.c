#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node *create_node(int data){
    struct Node *node=(struct Node*)malloc(sizeof(struct Node));
    if(node==NULL){
        printf("malloc fail");
        exit(1);
    }
    node->data=data;
    node->next=NULL;
    return node;
}

void append(struct Node **head_ptr,int data){
    struct Node *new_node=create_node(data);
    if(*head_ptr==NULL){
        *head_ptr=new_node;
        return;
    }

    struct Node *finger=*head_ptr;
    while(finger->next!=NULL){
        finger=finger->next;
    }
    finger->next=new_node;
}

void print_list(struct Node *list){
    struct Node *finger=list;
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

    print_list(head);

    return 0;
}


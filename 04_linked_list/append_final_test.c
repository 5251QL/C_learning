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

void append(struct Node **head,int data){
    struct Node *new_node=create_node(data);
    if(*head==NULL){
        *head=new_node;
        return;
    }

    struct Node *finger=*head;
    while(finger->next!=NULL){
        finger=finger->next;
    }
    finger->next=new_node;
    return;
}

void print_list(struct Node *head){
    struct Node *finger=head;
    while(finger!=NULL){
        printf("%d->",finger->data);
        finger=finger->next;
    }
    

}

int main(){
    struct Node *head=NULL;

    append(&head,10);
    append(&head,20);
    append(&head,30);

    print_list(head);

    return 0;
}

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

void append(struct Node **head,int data){
    struct Node *new=create_node(data);
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

void print_list(struct Node *list){
    struct Node *finger=list;
    while(finger!=NULL){
        printf("%d->",finger->data);
        finger=finger->next;
    }
    printf("NULL\n");
}

void delete(struct Node **head,int target){
    if(*head==NULL){
        printf("nothing\n");
        return;
    }

    struct Node *p=*head;
    struct Node *prev=NULL;

    while(p!=NULL&&p->data!=target){
        prev=p;
        p=p->next;
    }
    if(p==NULL){
        printf("nothing\n");
        return;
    }
    if(prev==NULL){
        *head=p->next;
    }else{
        prev->next=p->next;
    }
    free(p);
    printf("delete:%d\n",target);
}

int main(){
    struct Node *head=NULL;

    append(&head,10);
    append(&head,20);
    append(&head,30);
    append(&head,40);
    append(&head,50);
    print_list(head);

    delete(&head,10);
    print_list(head);

    delete(&head,30);
    print_list(head);

    delete(&head,50);
    print_list(head);

    delete(&head,99);

    return 0;
}
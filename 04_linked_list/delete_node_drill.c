#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node *create_node(int data){
    struct Node *node=(struct Node*)malloc (sizeof(struct Node));
    if(node==NULL){printf("malloc fail");}
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

void delete_node(struct Node **head_ptr,int target){
    if(*head_ptr==NULL){
        printf("there`s nothing %d\n",target);
        return;
    }

    struct Node *p=*head_ptr;
    struct Node *prev=NULL;

    while(p!=NULL&&p->data!=target){
        prev=p;
        p=p->next;
    }

    if(p==NULL){
        printf("nothing %d\n",target);
        return;
    }

    if(prev==NULL){
        *head_ptr=p->next;
    }else{
        prev->next=p->next;
    }

    free(p);
    printf("delete:%d\n",target);
}

int  main(){
    struct Node *head=NULL;

    append(&head,10);
    append(&head,20);
    append(&head,30);
    print_list(head);

    delete_node(&head,20);
    print_list(head);

    delete_node(&head,10);
    print_list(head);

    delete_node(&head,99);

    delete_node(&head,30);
    print_list(head);

    delete_node(&head,10);

    return 0;


}


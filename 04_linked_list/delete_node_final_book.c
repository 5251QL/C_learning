#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
    struct Node *next;
};

struct Node *create(int data){
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
    return;
}

void print(struct Node *list){
    struct Node *finger=list;
    while(finger!=NULL){
        printf("%d->",finger->data);
        finger=finger->next;
    }
    printf("NULL\n");
    return;
}

void delete_list(struct Node **head,int target){
    if(*head==NULL){
        printf("nothing");
    }

    struct Node *p=*head;
    struct Node *pb=NULL;

    while(p!=NULL&&p->data!=target){
        pb=p;
        p=p->next;
    }

    if(p==NULL){
        printf("nothing");
        return;
    }if(pb==NULL){
        *head=p->next;
    }else{
        pb->next=p->next;
    }
    printf("delete:%d\n",target);

}

int main(){
    struct Node *head=NULL;

    append(&head,1);
    append(&head,11);
    append(&head,111);
    append(&head,1111);
    append(&head,11111);

    print(head);

    delete_list(&head,1);
    print(head);

    delete_list(&head,111);
    print(head);

    delete_list(&head,11111);
    print(head);

    delete_list(&head,123456);

    return 0;
    
}
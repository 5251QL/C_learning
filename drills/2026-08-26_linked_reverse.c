#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *create(int data){
    struct Node *new_node=(struct Node *)malloc(sizeof(struct Node));
    if(new_node==NULL){
        printf("malloc fail\n");
        exit(1);
    }
    new_node->data=data;
    new_node->next=NULL;
    return new_node;
}

void append(struct Node **head,int data){
    struct Node *new_node=create(data);
    if(*head==NULL){
        *head=new_node;
        return;
    }

    struct Node *p=*head;
    while(p->next!=NULL){
        p=p->next;
    }
    p->next=new_node;
}

struct Node *reverse(struct Node *head){
    struct Node *prev=NULL;
    struct Node *curr=head;
    struct Node *next=NULL;

    while(curr!=NULL){
        next=curr->next;
        curr->next=prev;
        prev=curr;
        curr=next;
    }
    return prev;
}

void delete_node(struct Node **head,int target){
    if(*head==NULL){
        printf("nothing\n");
        return;
    }

    struct Node *p=*head;
    struct Node *prev=NULL;
    while(p!=NULL&&p->data!=target){
        prev=p;
        p=p->next;
    }if(p==NULL){
        printf("no target\n");
        return;
    } else if(prev==NULL){
        *head=p->next;
    }else{
        prev->next=p->next;
    }
    free(p);
    printf("delete %d\n",target);
}

void print(struct Node *head){
    struct Node *p=head;
    while(p!=NULL){
        printf("%d ",p->data);
        p=p->next;
    }
    printf("\n");

}

int main(){
    struct Node *head=NULL;

    append(&head,10);
    append(&head,20);
    append(&head,30);
    print(head);

    head=reverse(head);
    print(head);

    delete_node(&head,10);
    delete_node(&head,40);

    return 0;
}
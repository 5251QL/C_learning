#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *create_node(int data){
    struct Node *node=(struct Node *)malloc(sizeof(struct Node));
    if(node==NULL){
        printf("malooc fail");
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

void print(struct Node *list){
    struct Node *finger=list;
    while(finger!=NULL){
        printf("%d->",finger->data);
        finger=finger->next;
    }
    printf("NULL\n");
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
    }else if(prev==NULL){
        *head=p->next;
    }else{
        prev->next=p->next;
    }
    free(p);
    printf("delete:%d\n",target);
}

int main(){
    struct Node *head=NULL;

    append(&head,1);
    append(&head,2);
    append(&head,3);
    append(&head,4);
    append(&head,5);

    print(head);
    head=reverse(head);
    print(head);

    delete(&head,1);
    print(head);

    head=reverse(head);

    delete(&head,3);
    print(head);

    delete(&head,5);
    print(head);

    return 0;
}
#include <stdio.h>
#include <stdint.h>

#define RB_SIZE 16


typedef struct{
    int buf[RB_SIZE];
    uint16_t head;
    uint16_t tail;
}rb_t;

void rb_init(rb_t *rb){
    rb->head=0;
    rb->tail=0;
}

int rb_push(rb_t *rb,int data){
    if((rb->head+1)%RB_SIZE==rb->tail){
        return 0;
    }
    rb->buf[rb->head]=data;
    rb->head=(rb->head+1)%RB_SIZE;
    return 1;
}

int rb_pop(rb_t *rb,int *out){
    if(rb->head==rb->tail){
        return 0;
    }
    *out=rb->buf[rb->tail];
    rb->tail=(rb->tail+1)%RB_SIZE;
    return 1;
}

int rb_top(rb_t *rb,int *out){
    if(rb->head==rb->tail){
        return 0;
    }
    *out=rb->buf[rb->tail];
    return 1;
}

int main(){
    rb_t rb;
    rb_t rb2;
    int v;

    rb_init(&rb);

    rb_push(&rb,10);
    rb_push(&rb,20);
    rb_push(&rb,30);

    rb_pop(&rb,&v);
    printf("pop:%d\n",v);

    rb_top(&rb, &v);
    printf("top: %d\n", v);

    rb_pop(&rb, &v);
    printf("pop: %d\n", v);

    return 0;
}


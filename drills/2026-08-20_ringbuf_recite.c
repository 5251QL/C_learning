#include <stdio.h>
#include <stdint.h>

#define BUF_SIZE 4

typedef struct{
    uint8_t buf[BUF_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
}rb_t;

void rb_init(rb_t *rb){
    rb->head=0;
    rb->tail=0;
    rb->count=0;
    return;
}

uint8_t rb_write(rb_t *rb,uint8_t data){
    if(rb->count==BUF_SIZE){
        printf("[FULL] cannot write %d\n",data);
        return 0;
    }
    rb->buf[rb->tail]=data;
    rb->tail=(rb->tail+1)%BUF_SIZE;
    rb->count++;
    return 1;
}

uint8_t rb_read(rb_t *rb,uint8_t *out){
    if(rb->count==0){
        printf("[EMPTY] nothing to read\n");
        return 0;
    }
    *out=rb->buf[rb->head];
    rb->head=(rb->head+1)%BUF_SIZE;
    rb->count--;
    return 1;
}

int main(){
    rb_t rb1;
    uint8_t v;
    rb_init(&rb1);

    rb_write(&rb1,10);
    rb_write(&rb1,20);
    rb_write(&rb1,30);
    rb_write(&rb1,40);
    rb_write(&rb1,50);

    while(rb_read(&rb1,&v)){
        printf("got %d\n",v);
    }

    return 0;
}


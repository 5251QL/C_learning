#include <stdio.h>
#include <stdint.h>

#define RB_SIZE 4
 
typedef struct{
    uint8_t buf[RB_SIZE];
    uint16_t head;
    uint16_t tail;
}rb_t;

void rb_init(rb_t *rb){
    rb->head=0;
    rb->tail=0;
}

uint8_t rb_write(rb_t *rb,uint8_t data){
    if((rb->tail+1)%RB_SIZE==rb->head){
        printf("[FULL] cannot write %d\n",data);
        return 0;
    }
    rb->buf[rb->tail]=data;
    rb->tail=(rb->tail+1)%RB_SIZE;
    return 1;
}

uint8_t rb_read(rb_t *rb,uint8_t *out){
    if(rb->head==rb->tail){
        printf("[EMPTY] nothing to read \n");
        return 0;
    }
    *out=rb->buf[rb->head];
    rb->head=(rb->head+1)%RB_SIZE;
    return 1;
}

void rb_dump(const rb_t *rb){
    printf("head=%d tail=%d\n",rb->head,rb->tail);
}

int main(){
    rb_t rb2;
    rb_init(&rb2);
    uint8_t v;

    printf("==write 10 20 30 40==\n");
    rb_write(&rb2,10);
    rb_write(&rb2,20);
    rb_write(&rb2,30);
    rb_write(&rb2,40);
    rb_dump(&rb2);

    rb_write(&rb2,50);
    rb_dump(&rb2);

    printf("==read 2 items==\n");
    if(rb_read(&rb2,&v)) printf("  got%d\n",v);
    if(rb_read(&rb2,&v)) printf("  got%d\n",v);
    rb_dump(&rb2);

    printf("==write 99 88==\n");
    rb_write(&rb2,99);
    rb_write(&rb2,88);
    rb_dump(&rb2);

    printf("==read all==\n");
    while(rb_read(&rb2,&v)) printf("  got%d\n",v);
    rb_dump(&rb2);

    return 0;
}

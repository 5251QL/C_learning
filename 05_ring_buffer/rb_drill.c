#include <stdio.h>

#define SIZE 8

/*int buf[SIZE]; //存放数据的数组
int write_pos=0; //下一次“写”的位置
int read_pos=0; //下一次“读”的位置

//写一个数据到缓冲区
void rb_write(int data){
    //如果写满了覆盖旧数据(目前简单处理，不做判满)
    buf[write_pos]=data;
    write_pos=(write_pos+1)%SIZE;
}

//从缓冲区读一个数据
int re_read(void){
    //如果缓冲区空了，返回-1(简单处理)
    int data=buf[read_pos];
    read_pos=(read_pos+1)%SIZE;
    return data;
}

int main(){
    //写五个数据
    rb_write(10);
    rb_write(20);
    rb_write(30);
    rb_write(40);
    rb_write(50);
    rb_write(60);
    rb_write(70);
    rb_write(80);

    //读五个数据
    for (int i=0;i<8;i++){
        printf("read:%d\n",re_read());
    }

    return 0;
}*/

/*int fifo[SIZE];
int fifo_push;
int fifo_pop;

void write(int data){
    fifo[fifo_push]=data;
    fifo_push=(fifo_push+1)%SIZE;
}

int read(void){
    int data=fifo[fifo_pop];
    fifo_pop=(fifo_pop+1)%SIZE;
    return data;
}

int main(){
    write(100);
    write(200);
    write(300);

    for(int i=0;i<3;i++){
        printf("read:%d\n",read());
    }
}*/

/*int queue[SIZE];
int enqueue;
int dequeue;

void write(int data){
    queue[enqueue]=data;
    enqueue=(enqueue+1)%SIZE;
}

int read(void){
    int data=queue[dequeue];
    dequeue=(dequeue+1)%SIZE;
    return data;
}

int main(){
    write(1);
    write(2);
    write(3);
    write(4);
    write(5);
    write(6);
    write(7);
    write(8);

    for(int i=0;i<8;i++){
        printf("read:%d\n",read());
    }
}*/

/*int ring[SIZE];
int ring_put=0;
int ring_get=0;

void write(int data){
    ring[ring_put]=data;
    ring_put=(ring_put+1)%SIZE;

}

int read(void){
    int data=ring[ring_get];
    ring_get=(ring_get+1)%SIZE;
    return data;
}

int main(){
    write(99);
    write(88);
    write(77);
    write(66);

    for(int i=0;i<4;i++){
        printf("read:%d\n",read());
    }

    return 0;

}*/

int buf[SIZE];
int rb_write=0;
int rb_read=0;

void write(int data){
    buf[rb_write]=data;
    rb_write=(rb_write+1)%SIZE;
}

int read (void){
    int data=buf[rb_read];
    rb_read=(rb_read+1)%SIZE;
    return data;
}

int main(){
    write(1);
    write(2);
    write(3);
    write(4);
    write(5);

    for (int i=0;i<7;i++){
        printf("read:%d\n",read());
    }

    return 0;
}
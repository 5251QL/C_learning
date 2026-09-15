#include <stdio.h>
#define SIZE 10

int a[SIZE];
int write_pos=0;
int read_pos=0;

void rb_write(int data){
    a[write_pos]=data;
    write_pos=(write_pos+1)%SIZE;
}

int read(void){
    int data=a[read_pos];
    read_pos=(read_pos+1)%SIZE;
    return data;
}

int main(){
    rb_write(10);
    rb_write(20);
    rb_write(30);
    rb_write(40);
    rb_write(50);

    for(int i=0;i<8;i++){
        printf("read:%d\n",read());
    }


    return 0;
}
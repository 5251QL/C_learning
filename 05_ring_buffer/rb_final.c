#include <stdio.h>

#define SIZE 8

int buf[SIZE];
int write_pos=0;
int read_pos=0;

void rb_write(int data){
    buf[write_pos]=data;
    write_pos=(write_pos+1)%SIZE;
}

int rb_read(void){
    int data=buf[read_pos];
    read_pos=(read_pos+1)%SIZE;
    return data;
}

int main(){
    rb_write(11);
    rb_write(22);
    rb_write(33);
    rb_write(44);
    rb_write(55);

    for(int i=0;i<8;i++){
        printf("read:%d\n",rb_read());
    }

    return 0;
}
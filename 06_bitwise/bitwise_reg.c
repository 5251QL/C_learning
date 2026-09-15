#include <stdio.h>

int main(){
    unsigned int reg=0x00;

    //1、把PA5设成输出模式————把第5位置1
    reg|=(1<<5);
    printf("after:0x%08x (bit5=1)\n",reg);

    //2、关闭PA5————把第5位清零
    reg&=~(1<<5);
    printf("after:0x%08x (bit5=0)\n",reg);

    //3、判断按键PA3是否按下————读第3位
    reg=0x08; //模拟PA3高电平
    if(reg&(1<<3)){
        printf("PA3=1(bit3=1)\n");
    }else{
        printf("PA3=0(bit3=0)\n");
    }

    return 0;
}
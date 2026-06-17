#include <stdio.h>

int main(){
//把第n位置1
//REG|=(1<<n)
unsigned int reg=0x00;
reg |=(1<<3);
printf("1-0x%02x(0x08)\n",reg);

//把第n位清零
//REG&=~(1<<n)
reg=0xFF;
reg&=~(1<<2);
printf("2-0x%02x(0xFB)\n",reg);

//判断第n位是1还是0
//if(REG&(1<<n))
reg=0x0A;
printf("3-3:%d(1),1:%d(1),0:%d(0)\n",(reg>>3)&1,(reg>>1)&1,(reg>>0)&1);
}
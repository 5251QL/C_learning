#include <stdio.h>

int main(){
    unsigned int num=0;
    unsigned int result=0;

    //1.把第n位置1
    //口诀：num|(1<<n)
    num=0x00; //0000 0000
    result=num|(1<<3); //把第三位置1
    printf("1.0x%02x | (1<<3)=0x%02x(0x08)\n",num,result);

    //2.把第n位清零
    //口诀：num&~(1<<n)
    num=0xFF; //1111 1111
    result=num&~(1<<2); //把第二位清零
    printf("2.0x%02x &~ (1<<2)=0x%02x(0xFB)\n",num,result);

    //3.翻转第n位
    //口诀：num^(1<<n)
    num=0x0f; //0000 1111
    result=num^(1<<0); //翻转第零位
    printf("3.0x%02x ^ (1<<0)=0x%02x(0x0E)\n",num,result);

    //4.判断第n位是0还是1
    //口诀：(num>>n)&1
    num=0x0A; //0000 1010
    int bit3=(num>>3)&1; //第三位
    int bit1=(num>>1)&1; //第一位
    printf("4.0x%02x 3=%d(1), 1=%d(1)\n",num,bit3,bit1);

    //5.提取低四位
    //口诀：num&0x0F
    num=0xAB;
    result=num&0x0F;
    printf("5.0x%02x & 0x0F=0x%02x(0x0B)\n",num,result);

    //6.高低字节交换（16位数）
    //口诀：(num<<8(num>>8)
    unsigned short word=0xAB12;
    unsigned short swapped=(word<<8)|(word>>8);
    printf("6.0x%04x after:0x%04x(0x12AB\n",word,swapped);

    //7.判断奇偶
    //口诀：num&1(1为奇，0为偶)
    int a=7,b=8;
    printf("7.%d & 1=%d(ji)  %d & 1=%d(ou)\n",a,a&1,b,b&1);

    //8.乘除2的幂(用移位代替乘除)
    //口诀:num<<n等价于num*(2^n)
    //     num>>n等价于num/(2^n)
    num=3;
    printf("8.3<<2=%d(=12) 3<<3=%d(=24) 24>>2=%d(=6)\n",
            num<<2,num<<3,24>>2);

    return 0;
}
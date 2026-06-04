#include <stdio.h>
#include <stdlib.h>

int main(){
    system("chcp 65001");
    int age;

    printf("how old are you?\n");
    printf(">");
    scanf("%d",&age);
    printf("your answer is:%d\n",age);
    system("pause");

    int a,b;
    printf("请输入两个整数，中间用空格分开");
    scanf("%d %d",&a,&b);
    printf("%d+%d=%d\n",a,b,a + b);

    return 0;

}
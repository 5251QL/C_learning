#include <stdio.h>

int main(){
    int a,b;
    char op;

    printf("请输入算式：");
    scanf("%d %c %d",&a,&op,&b);

    if(op=='+'){
        printf("%d + %d=%d",a,b,a+b);
    }else if(op=='-'){
        printf("%d - %d=%d",a,b,a-b);
    }else if(op=='*'){
        printf("%d * %d=%d",a,b,a*b);
    }else if(op=='/'){
        if(b==0){
            printf("error:division by zero.");
        }else{
            printf("%d / %d=%d",a,b,a/b);
        }
        
    }else{
        printf("不认识的运算符：%c\n",op);
    }

    return 0;
}
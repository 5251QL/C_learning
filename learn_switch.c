#include <stdio.h>

int main(){
    int a,b;
    char op;

    printf("shu ru suan shi:");
    scanf("%d %c %d",&a,&op,&b);

    switch(op){
        case '+':
        printf("%d+%d=%d",a,b,a+b);
        break;
        case '-':
        printf("%d-%d=%d",a,b,a-b);
        break;
        case '*':
        printf("%d*%d=%d",a,b,a*b);
        break;
        case '/':
        if(b==0){
            printf("error");
        }else{
                printf("%d/%d=%d",a,b,a/b);
            }
        
        break;
        default:printf("bu ren shi");
        
         }
         
return 0;
    }

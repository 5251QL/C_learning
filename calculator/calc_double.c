#include <stdio.h>

int main(){
    double a,b;
    char op;

    printf("shu ru suan shi");
    scanf("%lf%c%lf",&a,&op,&b);
    printf("DEBUG:a=%g,op=%c,b=%g\n",a,op,b);

    switch(op){
        case '+':printf("%g+%g=%g\n",a,b,a+b);break;
        case '-':printf("%g-%g=%g\n",a,b,a-b);break;
        case '*':printf("%g*%g=%g\n",a,b,a*b);break;
        case '/':
        if(b==0.0){
            printf("error");
        }else{
            printf("%g/%g=%g",a,b,a/b);
        }
        break;
        default:printf("bu ren shi");break;

    }
    return 0;
}
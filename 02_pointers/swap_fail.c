#include <stdio.h>

void swap_fail(int a,int b){
    int temp=a;
    a=b;
    b=temp;
    printf("jiao huan hou (a,b):a=%d,b=%d\n",a,b);

}

int main(){
    int x=10,y=20;

    printf("before:x=%d,y=%d\n",x,y);
    swap_fail(x,y);
    printf("after:x=%d,y=%d\n",x,y);

    return 0;
}
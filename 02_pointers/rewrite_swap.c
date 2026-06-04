#include <stdio.h>

void swap(int *a,int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

int main(){
    int x ,y;
    printf("shu ru liang ge shu zi:");
    scanf("%d %d",&x,&y);
    swap(&x,&y);
    printf("after:x=%d y=%d\n",x,y);

    return  0;

}
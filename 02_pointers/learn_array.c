#include  <stdio.h>

int main(){
    int arr[]={10,20,30,40,50};
    int *p=arr;

    for(int i=0;i<5;i++){
        printf("p->:%p,zhi:%d\n",p,*p);
        p++;
    }

    return 0;
}

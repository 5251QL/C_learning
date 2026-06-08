#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int *p=(int *)malloc(sizeof(int));
    if(p==NULL){
        printf("malloc fail\n");
        return 1;
    }
    *p=42;
    printf("malloc`s int:%d\n",*p);
    free(p); //用完必须还

    //动态分配一个结构体
    struct Student{
    char name[20];
    int age;};
    struct Student *ps=(struct Student *)malloc(sizeof(struct Student));
    if (ps==NULL){printf("fail\n");return 1;}
    ps->age=21;
    strcpy(ps->name,"zhangsan");
    printf("dongtaifenpei age:%d\n",ps->age);
    printf("dongtaifenpei name:%s\n",ps->name);
    free(ps);

    return 0;
}
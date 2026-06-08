#include <stdio.h>

struct S{
    char name[20];
    int age;
    double score;
};

struct S s1={"wangwu",20,88.0};

void brithday_pointer(struct S *p){
    p->age++;

}

int main(){
    printf("%s %d %.1f\n",s1.name,s1.age,s1.score);
    brithday_pointer(&s1);
    printf("%s %d %.1f\n",s1.name,s1.age,s1.score);

    return 0;
}
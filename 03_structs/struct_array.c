#include <stdio.h>

struct Student{
    char name[20];
    int age;
    double score;
};

int main(){
    struct Student class[3]={
        {"AAA",20,85.0},
        {"BBB",21,92.5},
        {"CCC",19,78.0}
    };
    //一、下标指针
    /*printf("---xiabiaobianli---\n");
    for (int i=0;i<3;i++){
        printf("%s:%dsui,%.1ffen\n",
        class[i].name,class[i].age,class[i].score);
    }*/
   //二、指针遍历
    printf("---xiabiaozhizhen---\n");
    for (int i=0;i<3;i++){
        printf("%s:%dsui,%.1ffen\n",
        class[i].name,class[i].age,class[i].score);
    }

    return 0;
}
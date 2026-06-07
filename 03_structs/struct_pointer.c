#include <stdio.h>

struct Student{
    char name[20];
    int age;
    double score;
};

int main(){
    struct Student s1={"Lisi",21,92.0};
    struct Student *p=&s1;
    
    printf("(*p).name=%s\n",(*p).name);
    printf("p->name=%ss\n",p->name);
    printf("p->age=%d\n",p->age);
    printf("p->score=%.1f\n",p->score);

    p->score=95.5;
    printf("change s1.score=%.1f\n",s1.score);
    
    return 0;

}
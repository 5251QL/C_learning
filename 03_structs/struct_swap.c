#include <stdio.h>

struct Student {
    char name[20];
    int age;
    double score;
};

void print_student(struct Student s){
    printf("name:%s,age:%d,score:%.1f\n",s.name,s.age,s.score);
}

void brithday_value(struct Student s){ //传值
    s.age++;
    printf("(1)age:%d\n",s.age);
}

void brithday_pointer(struct Student *s){ //传指针
    s->age++;
    printf("(2)age:%d\n",s->age);
}

int main(){
    struct Student s1= {"wangwu",20,8.0};

    printf("yuanshi:");
    print_student(s1);

    //实验一：传值
    brithday_value(s1);
    printf("1_after:");
    print_student(s1); //<--年龄不变

    //实验二：传指针
    brithday_pointer(&s1);
    printf("2_after:");
    print_student(s1); //<--年龄+1

    return 0;
}
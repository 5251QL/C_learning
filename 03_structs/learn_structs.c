#include <stdio.h>

/*struct Student{
    char name[20];
    int age;
    double score;
};

int main(){
    struct Student s1={"zhangsan",20,87.5};

    printf("name:%s\n",s1.name);
    printf("age:%d\n",s1.age);
    printf("score:%.1f\n",s1.score);

    return 0;
}*/

struct Device {
    int id;
    double voltage;
    int status;
};

int main(){
    struct Device d1 ={1,3.3,1};

    printf("shebei %d:dianya=%.1fV,zhuangtai=%d\n",d1.id,d1.voltage,d1.status);
    
    return 0;
}
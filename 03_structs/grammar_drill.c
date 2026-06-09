#include <stdio.h>

/*//方式1：定义类型，再声明变量
struct Point{
    int x;
    int y;
};

//方式2：定义类型的同时声明变量
struct Rectangle{
    int width;
    int height;
} r1={10,20};

int main(){
    //方式1：先定义类型，再声明变量，再赋值
    struct Point p1;
    p1.x=3;
    p1.y=5;
    printf("p1:(%d,%d)\n",p1.x,p1.y);

    //方式2：定义类型的同时生命变量并初始化变量
    printf("r1:%d*%d\n",r1.width,r1.height);

    //方式3：初始化列表
    struct Point p2={7,8};
    printf("p2:(%d,%d)\n",p2.x,p2.y);

    return 0;
}*/

/*struct A{
    int x,y;
};

struct B{
    int a,b;
}bb={1,2};

int main(){
    struct A aa={11,22}; //初始化？
    printf("aa:(%d,%d)\n",aa.x,aa.y);

    printf("bb:(%d-%d)\n",bb.a,bb.b); //声明赋值同步

    struct A cc; //声明后再赋值
    cc.x=33;
    cc.y=44;
    printf("cc:(%d,%d)\n",cc.x,cc.y);

    return 0;
}*/

/*struct student{
    char name[20];
    int age;
    double score;
} class[3]={
    {"zhangsan",712,88.1},
    {"lisi",20,89.5},
    {"wangwu",19,87.9}
};



int main(){

   struct student s1=class[0];
   struct student *ps=&s1;

   //.的用法，左侧必须是结构体变量
   printf("s1.age=%d\n",s1.age);

   //->用法：左侧必须是指向结构体的指针
   printf("ps->age=%d\n",ps->age);

   //->和(*ps).是等价的
   printf("(*ps).age=%d\n",(*ps).age);

   //通过指针修改原结构体
   ps->age++;
   printf("修改后 s1.age=%d\n",s1.age);

   return 0;
}*/

/*struct A{
    int a,aa;
}AA={10,20};

int main(){
    struct A *p=&AA;

    printf("num:(%d,%d)\n",AA.a,AA.aa);

    printf("num:(%d,%d)\n",p->a,p->aa);

    printf("num:(%d,%d)\n",(*p).a,(*p).aa);
    p->a=100;
    p->aa=1000;
    printf("after:%d,%d\n",p->a,p->aa);
}*/

#include <stdlib.h>

/*struct Device{
    int id;
    double voltage;
};

int main(){
//标准句式
struct Device *pd=(struct Device *)malloc(sizeof(struct Device));
if (pd==NULL){
    printf("malloc fail");
    return 1;
}

pd->id=1;
pd->voltage=3.3;
printf("shebei %d:%.1fV\n",pd->id,pd->voltage);

return 0;
}*/

/*struct Sensor {
    int id;
    double temp;
};

int main(){
    struct Sensor *ps=(struct Sensor *)malloc(sizeof(struct Sensor));
    if(ps==NULL){
        printf("malloc fail");
    }

    ps->id=2;
    ps->temp=25.5;
    printf("id:%d,temp:%.1f",ps->id,ps->temp);

    return 0;
}*/

/*struct Motor {
    int id;
    int speed;
};

int main(){
    struct Motor *pa=(struct Motor *)malloc(sizeof(struct Motor));
    if (pa==NULL){
        printf("malloc fail");
    }

    pa->id=3;
    pa->speed=1200;
    printf("id:%d,speed:%d",pa->id,pa->speed);

    return 0;
}*/

/*struct LED{
    int pin;
    int state;
};

int main(){
    struct LED *pq=(struct LED*)malloc(sizeof(struct LED));
    if(pq==NULL){
        printf("malloc fail");
    }

    pq->pin=13;
    pq->state=1;
    printf("pin:%d,state:%d\n",pq->pin,pq->state);

    return 0;
}*/

/*struct Device {
    int id;
    double voltage;
};

int main(){
    struct Device *pd=(struct Device*)malloc(sizeof(struct Device));
    if(pd==NULL){
        printf("malloc fail");
    }

    pd->id=1;
    pd->voltage=3.3;

printf("ID:%d VOLTAGE:%.1f\n",pd->id,pd->voltage);
return 0;
}*/

struct Node{
    int data;
    struct Node *next;
};

int main(){
    struct Node *p=(struct Node *)malloc(sizeof(struct Node));
    if(p==NULL){
        printf("malloc fail");
    }

    p->data=42;
    p->next=NULL;
    printf("jiedian:%d\n",p->data);

    free(p);

    return 0;
}


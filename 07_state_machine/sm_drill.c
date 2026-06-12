#include <stdio.h>

enum LightState{
    OFF,
    ON,
    BLINK
};

int main(){
    enum LightState state=OFF;
    char input;

    printf("state of light\n");
    printf("'p'->key enter,'q'->quit\n");
    printf("first state:OFF\n\n");

    while(1){
        printf("state:[OFF/ON/BLINK] now=%d,output:",state);
        scanf("%c",&input);

        if(input=='q')break;

        switch(state){
            case OFF:
                 if(input=='p'){
                    state=ON;
                    printf("->light up!\n");
                 }
                 break;

            case ON:
                 if(input=='p'){
                    state=BLINK;
                    printf("->blink!\n");
                 }
                 break;

            case BLINK:
                 if(input=='p'){
                    state=OFF;
                    printf("->light off!\n");
                 }
                 break;
        }
    }

    return 0;
}
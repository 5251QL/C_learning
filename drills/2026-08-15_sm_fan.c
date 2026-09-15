#include <stdio.h>

enum{OFF,LOW,HIGH};

int main(void){
    int state=OFF;
    char input;

    printf("press 'p' th switch,'q' to quit\n");

    while(1){
        scanf("%c",&input);

        if(input=='q'){
            printf("quit\n");
            break;
        }

        switch(state){
            case OFF:
                if(input=='p'){
                    state=LOW;
                    printf("state:LOW\n");
                }
            break;
            case LOW:
                if(input=='p'){
                    state=HIGH;
                    printf("state:HIGH\n");
                }
            break;
            case HIGH:
                if(input=='p'){
                    state=OFF;
                    printf("state:OFF\n");
                }
            break;
            default:
            break;
        }
    }
    return 0;
}
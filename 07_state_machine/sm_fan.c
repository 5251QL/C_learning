#include <stdio.h>

enum fanstate{
    OFF,
    LOW,
    HIGH,
};

int main(){
    enum fanstate state=OFF;
    char input;

    printf("state:[OFF/LOW/HIGH]\n");
    printf("key_enter:'p',quit:'q'\n");

    while(1){
        
        scanf(" %c",&input);

        if(input=='q'){
            printf("quit");
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
        }

        
    }

    return 0;
}
#include <stdio.h>

/* ① 三个状态，用 enum 起名字（自动变成 0/1/2）*/
enum { OFF, LOW, HIGH };

int main(void) {
    int state = OFF;        /* 状态变量：从 OFF 开始 */
    char input;

    printf("press 'p' to switch, 'q' to quit\n");

    while (1) {
        scanf(" %c", &input);       /* ② 事件：读按键 */

        if (input == 'q') {         /* 退出 */
            printf("quit\n");
            break;
        }

        switch (state) {            /* ③ 看当前状态 */
        case OFF:
            if (input == 'p') {     /* 在这个状态下，遇到 p */
                state = LOW;        /* 转移到 LOW */
                printf("state: LOW\n");
            }
            break;
        case LOW:
            if (input == 'p') {
                state = HIGH;
                printf("state: HIGH\n");
            }
            break;
        case HIGH:
            if (input == 'p') {
                state = OFF;        /* 转一圈回来 */
                printf("state: OFF\n");
            }
            break;
        default:
            break;
        }
    }
    return 0;
}

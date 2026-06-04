#include <stdio.h>

int main()
{
    double num1, num2, result;
    char op;

    printf("Simple Calculator\n");
    printf("Format: number operator number (e.g. 15 + 23)\n");
    printf("Enter q to quit\n\n");

    while (1) {
        printf("> ");
        
        // ----- 你的任务1：读取第一个数 -----
        scanf("")// 提示：用 scanf("%lf", &num1)
        // 如果用户输入了 q，scanf 会失败，怎么检测？
        // 如果检测到失败，用 getchar() 读一下是不是 'q'
        // 如果是 q，打印 "Goodbye."，然后跳出循环
        // 如果不是 q，打印 "Invalid input."，然后 continue
        
        
        // ----- 你的任务2：读取运算符 -----
        // 提示：scanf(" %c", &op)
        // 注意 %c 前面有个空格，用来吃掉上次输入留下的换行
        
        
        // ----- 你的任务3：读取第二个数 -----
        // 提示：scanf("%lf", &num2)
        
        
        // ----- 你的任务4：根据运算符计算结果 -----
        // 提示：用 switch(op)
        // case '+': result = num1 + num2; break;
        // case '-': ...
        // case '*': ...
        // case '/': 
        //     你的任务5：这里要检查 num2 是不是 0
        //     如果是0，打印 "Error: Division by zero"
        //     然后跳过打印结果（怎么跳过？）
        //     如果不是0，正常计算
        
        
        // ----- 你的任务6：打印结果 -----
        // 提示：printf("= %g\n", result);
    }

    return 0;
}
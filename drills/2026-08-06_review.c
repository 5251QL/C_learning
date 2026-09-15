#include "stm32f1xx_hal.h"


uint8_t ringbuf_is_empty(void){     // head==tail
    return (head==tail);
}   

uint8_t ringbuf_is_full(void){      // (head+1)%SIZE==tail
    return ((head+1)%BUF_SIZE==tail);
}    

uint8_t ringbuf_write(uint8_t d){       // 先判满再写
    if((head+1)%BUF_SIZE==tail){
        return 0;
    }
    buf[head]=d;
    head=(head+1)%BUF_SIZE;
    return 1;
} 

uint8_t ringbuf_read(uint8_t *d){       // 先判空再读
    if(head==tail){
        return 0;
    }
    *d=buf[tail];
    tail=(tail+1)%BUF_SIZE;
    return 1;
} 


HAL_UART_Receive → if('1') → LED亮+ON → else if('0') → LED灭+OFF

int main(){
    HAL_UART_Receive(&g_uart_handle,&rx_byte,1,5000);  //'?'应该写什么？uint8_t *pData？这是数据的存储地址？我可以随意命名？还是什么别的？
    
    while (1){
        if(rx_byte=='1'){
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
            HAL_UART_Transmit(&g_uart_handle,(uint8_t *)"ON\r\n",4,1000);
        }
        else if(rx_byte=='0'){
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
            HAL_UART_Transmit(&g_uart_handle,(uint8_t *)"OFF\r\n",5,1000);
        }
    }
}


void led_init(void){    // PA8 推挽输出上拉
    GPIO_InitTypeDef cfg={0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    cfg.Mode=GPIO_MODE_OUTPUT_PP;
    cfg.Pull=GPIO_PULLUP;
    cfg.Speed=GPIO_SPEED_FREQ_HIGH;
    cfg.Pin=LED0_PIN;     HAL_GPIO_Init(LED0_PORT,&cfg);    //PA8引脚上接的是LED0，那么如果我写GPIO_PIN_8？这一部分应该放在#define里？
    led_all_off;
}         
void key_is_pressed(void){      // 消抖+下降沿
    uint8_t raw=(HAL_GPIO_ReadPin(GPIOC,KEY0_PIN)==GPIO_PIN_RESET)?1:0;

    if(raw==1){
        delay_ms(30);
        if(HAL_GPIO_ReadPin(GPIOC,KEY0_PIN)==GPIO_PIN_RESET){
            last_stable=1;
            return 0;
        }
    }

    if(raw==0&&last_stable==1){
        last_stable=0;
        return 1;
    }
    last_stable=raw;
    return 0;
}   



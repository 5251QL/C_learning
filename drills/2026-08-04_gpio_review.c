void led_init(void){
    GPIO_InitTypeDef cfg={0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    cfg.Mode=GPIO_MODE_OUTPUT_PP;
    cfg.Pull=GPIO_PULLUP;
    cfg.Speed=GPIO_SPEED_HIGH;

    cfg.Pin=LED0_PIN;   HAL_GPIO_Init(LED0_PORT,&cfg);

    led_all_off();
}

void key_init(void){
    GPIO_InitTypeDef cfg={0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    cfg.Mode=GPIO_MODE_INPUT;
    cfg.Pull=GPIO_PULLUP;
    cfg.Pin=KEY0_PIN;   HAL_GPIO_Init(KEY0_PORT,&cfg);
}

uint8_t key_is_pressed(void){
    uint8_t raw=(GPIO_ReadPin(KEY0_PORT,&KEY0_PIN)==GPIO_PIN_RESET)?1:0;

    if(raw==1){
        delay_ms(30);
        if(GPIO_ReadPin(KEY0_PORT,KEY0_PIN)==GPIO_PIN_RESET){
            last_stable=1;
            return 0;
        }
    }

    if(last_stable==1&&raw==0){
        last_stable=0;
        return 1;
    }
    last_stable=raw;
    return 0;
}
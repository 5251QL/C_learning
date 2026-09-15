void led_int(void){

    GPIO_InitTypedef cfg={0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    cfg.Mode=GPIO_MODE_OUTPUT_PP;
    cfg,Pull=GPIO_PULLUP;
    cfg.Speed=GPIO_SPEED_FREQ_HIGH;

    cfg.Pin=LED0_PIN;   GPIO_Init(LED0_PORT,&cfg);
    cfg.Pin=LED1_PIN;   GPIO_Init(LED1_PORT,&cfg);
    cfg.Pin=LED2_PIN;   GPIO_Init(LED2_PORT,&cfg);
    cfg.Pin=LED3_PIN;   GPIO_Init(LED3_PORT,&cfg);
    cfg.Pin=LED4_PIN;   GPIO_Init(LED4_PORT,&cfg);

    led_all_off();

}
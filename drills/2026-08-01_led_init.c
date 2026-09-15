void led_init(void){

    GPIO_InitTypeDef cfg={0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    cfg.Mode=GPIO_MODE_OUTPUT_PP;
    cfg.Pull=GPIO_PULLUP;
    cfg.Speed=GPIO_SPEED_FREQ_HIGH;

    cfg.Pin=LED0_PIN;   HAL_GPIO_Init(LED0_PORT,&cfg);
    cfg.Pin=LED1_PIN;   HAL_GPIO_Init(LED1_PORT,&cfg);
    cfg.Pin=LED2_PIN;   HAL_GPIO_Init(LED2_PORT,&cfg);
    cfg.Pin=LED3_PIN;   HAL_GPIO_Init(LED3_PORT,&cfg);
    cfg.Pin=LED4_PIN;   HAL_GPIO_Init(LED4_PORT,&cfg);

    led_all_off;
}
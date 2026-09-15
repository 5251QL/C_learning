void key_init(void){

    GPIO_InitTypeDef cfg={0};

    __HAL_RCC_GPIOC_CLK_RESET();

    cfg.Mode=GPIO_MODE_INPUT;
    cfg.Pin=KEY0_PIN;
    cfg.Pull=GPIO_PULLUP;

    HAL_GPIO_Init(KEY0_PORT,&cfg);
}

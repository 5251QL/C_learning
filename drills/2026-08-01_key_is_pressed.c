uint8_t key_is_pressed(void){
    uint8_t raw=(HAL_GPIO_ReadPin(KEY1_PORT,KEY1_PIN)==GPIO_PIN_SET)?1:0;

    if(raw==1){
        delay_ms(50);
        if(HAL_GPIO_ReadPin(KEY1_PORT,KEY1_PIN)== GPIO_PIN_SET){
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
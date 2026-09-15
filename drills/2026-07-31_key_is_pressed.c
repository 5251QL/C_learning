unit8_t key0_is_pressed(void){
    unit8_t raw=(HAL_GPIO_ReadPin(KEY0_PORT,KEY0_PIN)==GPIO_PIN_RESET)?1:0;

    if(raw==1){
        delay(30);
        if(HAL_GPIO_ReadPin(KEY0_PORT,KEY0_PIN)==GPIO_PIN_RESET){
            last_stable=1;
            return 0;
        }
    }

    if(last_stable==1&raw==0){
        last_stable=0;
        return 1;
    }

    last_stable=raw;
    return 0;
}
void led_all_off(){
    for(int i=0;i<LED_NUM;i++)
    HAL_GPIO_WritePin(
        led_table[i].port,
        led_table[i].pin,
        GPIO_PIN_SET
    );
}

void led_chase(unit16_t ms){
    for(int i=0;i<LED_NUM;i++){
        led_all_off();
        HAL_GPIO_WritePin(
            led_table[i].port,
            led_table[i].pin,
            GPIO_PIN_RESET
        );
        delay_ms(ms);
    }
}
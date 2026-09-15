while(1){
    HAL_UART_Receive(&g_uart1_handle,&rx_byte,1,5000);

    if(rx_byte=='1'){
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
        HAL_UART_Transmit(&g_uart1_handle,(uint8_t *)"ON\r\n",4,1000);
    }

    else if(rx_byte=='0'){
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
        HAL_UART_Transmit(&g_uart1_handle,(uint8_t *)"OFF\r\n",5,1000);
    }
}
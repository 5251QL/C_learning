#define BUF_SIZE 8
uint8_t buf[BUF_SIZE];
uint8_t head = 0;
uint8_t tail = 0;

uint8_t ringbuf_is_empty(void){ // 函数1：判断缓冲区是否为空
    return (head==tail);
}

uint8_t ringbuf_is_full(void){  // 函数2：判断缓冲区是否为满
    return ((head+1)%BUF_SIZE==tail);
}

uint8_t ringbuf_data_count(void){  // 函数3：获取有效数据量
    return (head-tail+BUF_SIZE)%BUF_SIZE;
}

uint8_t ringbuf_write(uint8_t d){   // 函数4：写一个字节（满则返回0）
    if((head+1)%BUF_SIZE==tail){
        return 0;
    }
    buf[head]=d;
    head=(head+1)%BUF_SIZE;
    return 1;
}

uint8_t ringbuf_read(uint8_t *d){    // 函数5：读一个字节（空则返回0）
    if(head==tail){
        return 0;
    }
    *d=buf[tail];
    tail=(tail+1)%BUF_SIZE;
    return 1;
}
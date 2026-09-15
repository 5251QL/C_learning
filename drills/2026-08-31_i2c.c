/**
 * 2026-08-31 I2C 验收题（独立编写第一轮）
 * 任务：填 5 个函数体（注释是提示不是答案）
 * 规则：不翻 myiic.c / 不看聊天记录里的代码
 * 编译：gcc -Wall -c 2026-08-31_i2c.c
 */
#include <stdint.h>

/* ===== BSP 替身宏（验收目标是时序逻辑，不是 HAL 层）===== */
#define IIC_SDA(x)      do { (void)(x); } while (0)   /* 写 SDA：1=放手(高) 0=拉低 */
#define IIC_SCL(x)      do { (void)(x); } while (0)   /* 写 SCL */
#define IIC_READ_SDA    1                             /* 读 SDA：返回 1(高)/0(低) */

static void iic_delay(void) { }                       /* 时序延时（占位） */

void iic_ack(void)  { IIC_SDA(0); iic_delay(); IIC_SCL(1); iic_delay(); IIC_SCL(0); iic_delay(); IIC_SDA(1); iic_delay(); }
void iic_nack(void) { IIC_SDA(1); iic_delay(); IIC_SCL(1); iic_delay(); IIC_SCL(0); iic_delay(); }

/* ① START：SCL 高电平期间，SDA 高→低（先复位 SDA=1，再 SCL=1，再拉低 SDA）*/
void iic_start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(0);
    iic_delay();
    IIC_SCL(0);
    /* 你的代码 */
}

/* ② STOP：SCL 高电平期间，SDA 低→高（先 SDA=0，再 SCL=1，再放手 SDA）*/
void iic_stop(void)
{
    IIC_SDA(0);
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(1);
    /* 你的代码 */
}

/* ③ wait_ack：放手 SDA → SCL 高 → 读 SDA
 *    读到低(0)=从机应答=成功 返回 0
 *    等 250 次还是高=超时 返回 1（记得 iic_stop() 收尾）*/
uint8_t iic_wait_ack(void)
{
    uint8_t wattime=0,rack=0;
    IIC_SDA(1);
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    while (IIC_READ_SDA){
        wattime++;
        if(wattime>250){iic_stop();rack=1;break;}
    }
    IIC_SCL(0);
    iic_delay();
    return rack;
    /* 你的代码 */
}

/* ④ send_byte：循环 8 次，高位先行（data & 0x80），换挡-采样，最后 data <<= 1 */
void iic_send_byte(uint8_t data)
{
    for(uint8_t n=0;n<8;n++){
        IIC_SCL(0);
        IIC_SDA((data&0x80)>>7);
        IIC_SCL(1);
        data<<=1;
    }
    IIC_SDA(1);
    /* 你的代码 */
}

/* ⑤ read_byte：循环 8 次，先 receive <<= 1 再 SCL 高读 SDA（1 则 receive++）
 *    读完后 ack=1 发 iic_ack()，ack=0 发 iic_nack() */
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i,receive=0;
    for(i=0;i<8;i++){
        receive<<=1;
        IIC_SCL(1);
        iic_delay();
        if(IIC_READ_SDA){
            receive++;
        }  
        IIC_SCL(0);
        iic_delay();  
    }
    if(!ack){iic_nack();}
    else iic_ack();    
    return receive;
    /* 你的代码 */
}

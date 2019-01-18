//====TWI总线测试
//====Loyer

#include <iom64v.h>
#include <port.h>
#include <delay.h>
#include <string.h>
#include <twi.h>
#include <default.h>

#define true 1
#define True 1
#define flase 0
#define False 0

byte count = 0;

#define SL_ADDR 0x03   //0B00000011;定义从机地址,使能广播接收
#define SR_INIT (TWCR = 0xC5);
//定义从机地址，使能TWI从机模式
void Slave_init(void)
{
		TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //接受主机呼叫地址，TWI使能，TWI中断使能
		TWAR = SL_ADDR;
		TWBR = 32;
}
#define BUFFER_SIZE 11  //缓冲区大小
byte buffer[BUFFER_SIZE];  //缓冲区
byte counter = 0;  //缓冲区中数据个数
#pragma interrupt_handler Slave_vect:34
void Slave_vect(void)
{
    switch (TW_STATUS)
    	{
    case TW_SR_SLA_ACK: //如果接收到主机SLA+W命令，并已返回了ACK
			SR_INIT;  //TWINT TWEA TWEN TWIE置位
			counter = 0;
    	break;
    case TW_SR_GCALL_ACK: //接收到广播地址，并已返回ACK
			SR_INIT;  //TWINT TWEA TWEN TWIE置位
			counter = 0;
    	break;
		case TW_SR_DATA_ACK: //已收到主机发来的一个数据，并已返回ACK
		    buffer[counter] = TWDR; //读取数据
			counter++;
			SR_INIT;
			break;
		case TW_SR_STOP:  //接收到STOP或重复START
			SR_INIT;
			break;
		////////////////////////////////////////////////////
		case TW_ST_SLA_ACK: //自己的SLA+R已经被接收ACK已返回
			counter = 0;
			TWDR = 0xF3; //从机发送
			//TWDR = buffer[counter];
			counter++;
			SR_INIT;
			break;
		case TW_ST_DATA_ACK: //数据已经发送接收到ACK
			counter++;
			TWDR = 0xF4;
			//TWDR = buffer[counter];
			SR_INIT;
			break;
		case TW_ST_DATA_NACK: ////数据已经发送接收到NOT_ACK
			SR_INIT;
			break;
		case TW_SR_GCALL_DATA_ACK:  //以前以广播方式被寻址,数据已经被接收ACK已返回
			buffer[counter] = TWDR;
			SR_INIT;
			counter++;
			break;			
    default: 
			SR_INIT;
    	break;
    	}
}
//定时器0初始化
void init_TIMER0_OVF(void) 
{
   TCCR0 = 0x06; //256分频
	TCNT0 = 256 - CRYSTAL/256/2*0.5;  //0.5s定时
	TIMSK |= (1<<TOIE0);  //定时器0中断使能
	SREG = 0x80;
}
#pragma interrupt_handler timer0_isr:17
void timer0_isr(void)
{
	for(count = 0; count < BUFFER_SIZE; count++)
	{
	    if(buffer[count] == 0xEF)
		   {
		       clr_bit(PORTB, PB6); //点亮LED
		   }
	}
	TCNT0 = 256 - CRYSTAL/256/2*0.5;  //重装0.5s定时
}

void init_CPU(void)
{
    DDRB = 0xFF;//做输出
		PORTB = 0xFF; //带上拉，避免LED误亮
		DDRD = 0x00; //SCL和SDA设置为输入
		PORTD = 0xFF; //内部上拉，可省去外部上拉电阻
		Slave_init();
		init_TIMER0_OVF();
}
void main(void)
{
    init_CPU();
		while(1)
		{
		    delay_nms(10);
		}
}
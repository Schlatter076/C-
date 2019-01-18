//====TWI总线测试
//====Loyer

#include <iom64v.h>
#include <port.h>
#include <delay.h>
#include <string.h>
#include <twi.h>

typedef unsigned int uint;
#define true 1
#define True 1
#define flase 0
#define False 0

//LED
char LED_String[] = "LEDSHOW:<----0123456789";

//INT2初始化
void INT2_init(void)
{
    EICRA = 0x20;  //中断2为下降沿触发
		EIMSK = 0x04;  //中断2使能
		SREG = 0x80;  //全局中断使能
}
#pragma interrupt_handler INT2_vect:4
void INT2_vect(void)
{
    static byte Shut_Down_6955 = 0x01;
		Shut_Down_6955 ^= 0x01;
		MAX6953_Write(0x04, 0x00 | Shut_Down_6955);  //0x00关断  0x01非关断
}
void main(void)
{
    byte i, j;
		DDRD = 0x00;
		PORTD = 0xFF;
		INT2_init();
		MAX6953_init();
		while(1)
		{
		    for(i = 0; i <= strlen(LED_String) - 4; i++)
			{
			    //将i开始的4个字符逐次发送到MAX6953的地址:0x20,0x21,0x22,0x23
				for(j = 0; j < 4; j++)
				{
				    MAX6953_Write(0x20 | j, (byte)LED_String[i + j]);
				}
				delay_nms(200);
			}
			delay_nms(1500);
		}
}
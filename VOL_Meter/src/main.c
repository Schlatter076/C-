/******************************************/
/*           广州旭东阪田电子有限公司     */
/*Project:     科牧自动测距FCT程序        */
/*Guest:                                  */
/*Name:             main.c                */
/*Mcu chip:         Atmega64              */
/*Main clock:       外部晶体11.0592MHz    */
/*Rtc clock:                              */
/*Author:                                 */
/*Create date:      2018.05.24            */
/*Design date:                            */
/*Complete date:                          */
/******************************************/
#include <iom64v.h>
#include <stdio.h>
#include <macros.h>
#include <port.h>
#include <default.h>
#include <delay.h>
#include <12864_LCD.h>
#include <beep.h>
#include <EEPROM.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <struct.h>

#define MY_SIZE 20

uchar Usart_com_buffer=0;
uint count = 0;
uint cmd_count = 0;

const uchar vol_1[] = {0xaa, 0x55, 0x04, 0xf6, 0xe8, 0x03, 0x01, 0xe5};
const uchar vol_2[] = {0xaa, 0x55, 0x03, 0xf5, 0x38, 0x01, 0x30};
/*============================================================*/	
/***********USART0接收中断服务函数 start**********************/ 
//USART接收缓冲区
#define RX_BUFFER_SIZE 30                  //接收缓冲区大小，可根据需要修改。
unsigned char rx_buffer[RX_BUFFER_SIZE];   //定义接收缓冲区
unsigned char rx_counter=0;                //定义rx_counter为存放在队列中的已接收到字符个数。

//定义一个标志位Usart0_RECVFlag1:=1表示串口0接收到了一个完整的数据包
//在port.h中定义

#pragma interrupt_handler usart0_rxc_isr:19  //接收中断服务程序
void usart0_rxc_isr(void)
{
    uchar status,data;
    status=UCSR0A;
    data=UDR0;
	if((flag1&(1<<Usart0_RECVFlag1))==0)   //判断是否允许接收一个新的数据包
	{
		if ((status & (USART0_FRAMING_ERROR | USART0_PARITY_ERROR | USART0_DATA_OVERRUN))==0)
        {
            rx_buffer[rx_counter] = data;
            rx_counter++;
			if(data==0x0A) 
			{
				set_bit(flag1,Usart0_RECVFlag1);
				rx_counter=0;
			}
			else
			{
				clr_bit(flag1,Usart0_RECVFlag1);
			}
        }
	}
}
/***************USART0接收中断服务函数 end**********************/ 
/*============================================================*/
/*============================================================*/	
/***************USART0发送中断服务函数 start********************/ 
#define TX_BUFFER_SIZE 11    
unsigned char tx_buffer[TX_BUFFER_SIZE];  
unsigned char tx_wr_index=0,tx_rd_index=0,tx_counter=0;

#pragma interrupt_handler usart0_txc_isr:21  //发送中断服务程序
void usart0_txc_isr(void)
{
    if (tx_counter)//队列不为空
    {
        --tx_counter;//出队列
        UDR0=tx_buffer[tx_rd_index];
        if (++tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
    }
}
/***********USART0发送中断服务函数 end**********************/ 

/*============================================================*/
/***********USART0发送一个字符函数 start**********************/ 
void USART0_putchar(unsigned char c)
{
    while (tx_counter == TX_BUFFER_SIZE);
    CLI();//#asm("cli")关闭全局中断允许
    if (tx_counter || ((UCSR0A & USART0_DATA_REGISTER_EMPTY)==0))//发送缓冲器不为空
    {
        tx_buffer[tx_wr_index]=c;//数据进入队列
        if (++tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;//队列已满
        ++tx_counter;
    }
    else
        UDR0=c;
    SEI(); //#asm("sei")打开全局中断允许
}
/***********USART0发送服务函数 end**********************/ 
/***************USART0初始化函数 start*************************/ 
void init_usart0(void)
{
    UCSR0B = 0x00; 
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);        //异步，8位数据，无奇偶校验，一个停止位，无倍速 
    UBRR0L = BAUD_L;                        //设定波特率 
    UBRR0H = BAUD_H; 
    UCSR0A = 0x00; 
    UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0); //0XD8  接收、发送使能， 开中断使能
}
/***************USART0初始化函数 end***************************/
#pragma interrupt_handler timer1_count_isr:15  
void timer1_count_isr(void) 
{ 
	for(cmd_count = 0; cmd_count < 8; cmd_count++)
	{
	    USART0_putchar(vol_1[cmd_count]);
	}
}
//定时器初始化
void init_TIMER1_OVF(void)
{
	TCCR1B=0x02;  //8分频
	TCNT1=65536-CRYSTAL/8*1.0;//0.5s定时
	TIMSK|=(1<<TOIE1);  //定时器使能
	SREG=0x80;      //全局使能中断
}

/***************系统初始化函数 start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //屏蔽INT0~INT1的所有外部中断
    clr_bit(SFIOR,PUD); //设置输入上拉电阻有效
    DDRA=0xff;//1是输出，0是输入
    PORTA=0x00; //LCD的数据口
	
    DDRB=0x20;  //1是输出，0是输入
    PORTB =0xff;//PB5上拉
	
    DDRC=0xff; 
    PORTC =0x00;

    DDRD  = 0x41;   //RXD1输入，且上拉有效  
	PORTD = 0xf4;   //TXD1输出。
	
	DDRE =0x0e;    //RXD0输入，且上拉有效
	PORTE =0x01;   //TXD0输出
	
	DDRF=0xff;
	PORTF =0x00;     
	
	DDRG =0xf0;  
	PORTG =0x00;  //PG3为产品感应器检测口//PG4继电器
	
	init_usart0();

    flag1=0;
    flag2=0;
    flag3=0;
    flagerr=0;
}
/***************系统初始化函数 end ***********/

/***************主函数 start *******************/
void main(void)
{
    init_cpu();    //初始化CPU
	init_TIMER1_OVF();
	beeperr3();
	while(1)
    {
		if((PIND & (1<<4)) == 0)
		{
		    delay_nms(5);
			if((PIND & (1<<4))==0)
			{
				for(cmd_count = 0; cmd_count < 8; cmd_count++)
				{
				    USART0_putchar(vol_1[cmd_count]);
				}
			}
		}
		if((PIND & (1<<5)) == 0)
		{
		    delay_nms(5);
			if((PIND & (1<<5))==0)
			{
				for(cmd_count = 0; cmd_count < 7; cmd_count++)
				{
				    USART0_putchar(vol_2[cmd_count]);
				}
			}
		}
	}
}



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

#define F_CPU 11059200UL
#define MY_SIZE 20
void USART0_putchar(unsigned char c);
void sendCMD(const uchar cmd[MY_SIZE]);

double Vdcdata = 0;
uchar str_vdc[25] =" ";
uchar Usart_com_buffer=0;
uint count = 0;
uint cmd_count = 0;
uchar data[MY_SIZE];
//电机位置
Pozition left_motor = {0, 0, 0, {0}};

const uchar str[] = "Fuck you!";
const uchar str_1[] = "Failed!";
const uchar start[MY_SIZE] = {0x60, 0x00, 0x81, 0x00, 0x68, 0x00, 0x84, 0x00, 0x79, 0x00, 0x80, 0x00, 0x67, 0x00, 0x62};
const uchar stop[MY_SIZE] = {0x60, 0x00, 0x83, 0x00, 0x50, 0x00, 0x67, 0x00, 0x83, 0x00, 0x87, 0x00, 0x67, 0x00, 0x62};
const uchar R_measure[MY_SIZE] = {0x60, 0x00, 0x67, 0x00, 0x83, 0x00, 0x68, 0x00, 0x90, 0x00, 0x83, 0x00, 0x49, 0x00, 0x62};
const uchar over[MY_SIZE] = {0x60, 0x00, 0x67, 0x00, 0x83, 0x00, 0x68, 0x00, 0x90, 0x00, 0x49, 0x00, 0x83, 0x00, 0x62};
const uchar ask[MY_SIZE] = {0x60, 0x00, 0x83, 0x00, 0x67, 0x00, 0x68, 0x00, 0x90, 0x00, 0x49, 0x00, 0x83, 0x00, 0x62};
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
void EEPROM_save(const uchar cmd[MY_SIZE])
{
    uint cmd_count = 0;
	for(cmd_count = 0; cmd_count < MY_SIZE; cmd_count++)
	{
		EEPROMwrite(cmd_count, ask[cmd_count]);
	}
}
/************USART0发送字符数组函数start****************/
void sendCMD(const uchar cmd[MY_SIZE]) 
{
	uint cmd_count = 0;
	for(cmd_count = 0; cmd_count < MY_SIZE; cmd_count++)
	{
		USART0_putchar(cmd[cmd_count]);
	}
}
/************USART0发送字符数组函数end****************/
/************USART0接收字符数组函数start****************/
void recieveCMD(void) 
{
    uchar n =0;
	//char *bool = "false";
	//delay_nms(100);
	//01 10 00 01 00 08 10 00 60 00 80 00 76 00 67 00 81 00 68 00 66 00 62 10 B9
	for(n = 0; n < 5; n++)
	{
	    delay_nms(100);
		if((int)rx_buffer[n] == 0x01 && (int)rx_buffer[n+1] == 0x10 && (int)rx_buffer[n+2] == 0x00 && (int)rx_buffer[n+3] == 0x01 && (int)rx_buffer[n+4] == 0x00
		&& (int)rx_buffer[n+5] == 0x08 && (int)rx_buffer[n+6] == 0x10 && (int)rx_buffer[n+7] == 0x00 && (int)rx_buffer[n+8] == 0x60 && (int)rx_buffer[n+9] == 0x00
		&& (int)rx_buffer[n+10] == 0x80 && (int)rx_buffer[n+11] == 0x00 && (int)rx_buffer[n+12] == 0x76 && (int)rx_buffer[n+13] == 0x00 && (int)rx_buffer[n+14] == 0x67
		&& (int)rx_buffer[n+15] == 0x00 && (int)rx_buffer[n+16] == 0x81 && (int)rx_buffer[n+17] == 0x00 && (int)rx_buffer[n+18] == 0x68 && (int)rx_buffer[n+19] == 0x00
		&& (int)rx_buffer[n+20] == 0x66 && (int)rx_buffer[n+21] == 0x00 && (int)rx_buffer[n+22] == 0x62 && (int)rx_buffer[n+23] == 0x10 && (int)rx_buffer[n+24] == 0xb9)
		{
		    delay_nms(600);
			sendCMD(start);
			delay_nms(2000);
			sendCMD(stop);
			delay_nms(2000);
			sendCMD(R_measure);
			count = 5;
			delay_nms(1000);
			sendCMD(over);
			//bool = "true";
			//break;
		}
	}
	if(count < 4) 
	{
	    for(n = 0; n < RX_BUFFER_SIZE; n++)
	    {
	        USART0_putchar(rx_buffer[n]);
	    }
	    count++;
	}
	
	//while(bool == "false");
} 
/************USART0接收字符数组函数end****************/
/***************USART01初始化函数 start*************************/ 
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

/*********处理数据函数 start *******************/
void fetch_data(void)			//处理数据	
{
	unsigned char n =0;
	uint i = 0;
	unsigned char num = 0;
	uchar *pstr1;
	uchar *pstr2;
	uchar *pstr3;
	pstr1 = rx_buffer;					//pstr1指向字符串头部
	pstr3=strchr(rx_buffer,'.');        //指向小数点
	if(pstr3==NULL) set_bit(flag1,error_flag1);  //=空，表示没有找到小数点
	pstr2 = strchr(rx_buffer, ' ');	    //pstr2指向空格
	if(pstr2 == NULL) set_bit(flag1, error_flag1);//找不到空格表示通信有错误
	num = pstr2 - pstr1;				//电阻字符串长度
	
	
	delay_nms(5);
	
	for(n=0;n<=11;n++)
	{
		str_vdc[n]=rx_buffer[n];
		//USART0_putchar(rx_buffer[n]);
	}
	
	Vdcdata = atof(str_vdc);
	delay_nms(1000);
	for(i = 0; i <= 11; i++) 
	{
	    USART0_putchar(str_vdc[i]);
	}
	//USART0_putchar(0x0d); //发送一个回车
	//USART0_putchar(0x0a); //发送一个换行
	//delay_nms(100);
	if(Vdcdata > 1)
	{
	    for(i = 0; i <= 9; i++)
	    {
	        USART0_putchar(str[i]);
	    } 
	}
    else
	{
	    for(i = 0; i <= 7; i++)
	    {
	        USART0_putchar(str_1[i]);
	    }
	}
	
}
/*********处理数据函数 end***********************/


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
	EEPROM_save(ask);
	//beeperr3();
	while(1)
    {
		//fetch_data();
		/**
		for(cmd_count = 0; cmd_count < MY_SIZE; cmd_count++)
		{
		    data[cmd_count] = EEPROMread(cmd_count);
			USART0_putchar(data[cmd_count]);
		}*/
		if((PIND & (1<<4)) == 0)
		{
		    delay_nms(5);
			if((PIND & (1<<4))==0)
			{
				//beeperr3();
				//01 10 00 01 00 08 10 00 60 00 80 00 76 00 67 00 81 00 68 00 66 00 62 10 B9
				/*
				delay_nms(600);
				sendCMD(start);
				delay_nms(2000);
				sendCMD(stop);
				delay_nms(2000);
				sendCMD(R_measure);
			    
			    for(cmd_count = 0; cmd_count < MY_SIZE; cmd_count++)
		        {
		            data[cmd_count] = EEPROMread(cmd_count);
			        USART0_putchar(data[cmd_count]);
		        }*/
				left_motor = getPozition(rx_buffer);
				delay_nms(50);
				for(cmd_count = 0; cmd_count < 20; cmd_count++)
				{
				    USART0_putchar(left_motor.buf[cmd_count]);
				}
			}
		}
		if((PIND & (1<<5)) == 0)
		{
		    delay_nms(5);
			if((PIND & (1<<5))==0)
			{
				while(count < 4) {
				   recieveCMD();
				}
			}
		}
	}
}



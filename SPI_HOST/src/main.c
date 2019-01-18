/******************************************/
/*           广州旭东阪田电子有限公司     */
/*Project:          SPI主机               */
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

#define TRUE 1
#define FALSE 0

//key
uchar key_now = 0;
uchar key_old = 0;
uchar key_code = 0;
uchar key_cnt = 0;
#define KEY_COUNTER 10
//SPI
char *master_statu = "transmit";  //主机状态，判定发送还是接收
const uchar ok[] = {0xf3, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0a};
uchar i = 0;

 /*SPI串行主机初始化*/
void SPI_MasterInit(void)
{
    //MOSI和SCK输出，其他为输入
	DDRB = (1 << DDB2)|(1 << DDB1);
	//使能主机模式，设置时钟速率fsc/16
	SPCR = (1 << SPE)|(1 << MSTR)|(1 << SPR0);
}
/*SPI主机发送一个字符*/
void SPI_MasterTransmit(unsigned char c)
{
    //启动数据传输
	SPDR = c;
	//等待发送结束
	while(!(SPSR & (1 << SPIF)));
}
/*SPI主机接收一个字符*/
unsigned char SPI_MasterReceive(void)
{
    SPDR = 0;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}
#define MASTER_RX_SIZE 11  //主机接收缓冲区大小
unsigned char master_rxBuffer[MASTER_RX_SIZE]; //主机接收缓冲区
unsigned char master_rxCounter = 0;  //已接收字符的个数
#define MASTER_TX_SIZE 11
unsigned char master_txBuffer[MASTER_TX_SIZE];
unsigned char master_txCounter = 0, master_wrIndex = 0, master_rdIndex = 0;
#pragma interrupt_handler SPI_TransEND:18
void SPI_TransEND(void)
{
    if(master_statu == "receive")
	{
	    uchar status,data;
	    status = SPSR; //SPI状态寄存器
	    data = SPDR; //SPI数据寄存器
	    if((flag2 & (1 << Master_RECVFlag2)) == 0)  //判断是否允许接收
	    {
			master_rxBuffer[master_rxCounter] = data;
			master_rxCounter++;
			//自定义接收方式
			switch(master_rxCounter)
			{
			    case 1: { //校验起始字节
				    if(data != 0xF3)  master_rxCounter = 0;
				}break;
				case 2: {
				    if(data != 0xF4)  master_rxCounter = 0;
				}break;
				case 11: {//校验结束字符
				    master_rxCounter =0;
					if(data == 0x0A)  set_bit(flag2, Master_RECVFlag2);
				}break;
				default:break;
			}
		}
	}
	if(master_statu == "transmit")
	{
	    if(master_txCounter) //队列不为空
		{
		    --master_txCounter; //出队列
			SPDR = master_txBuffer[master_rdIndex];
			if(++master_rdIndex == MASTER_TX_SIZE)  master_rdIndex = 0;			
		}
	}
}
//SPI发送一个字符
void SPI_putchar(unsigned char c)
{
    while(master_txCounter == MASTER_TX_SIZE);//等待数据填入发送缓冲区
	CLI();//关闭全局中断使能
	if(master_txCounter || ((SPSR & (1 << SPIF)) == 0)) //缓冲区不为空
	{
	    master_txBuffer[master_wrIndex] = c;
		if(++master_wrIndex == MASTER_TX_SIZE)  master_wrIndex = 0;
		++master_txCounter;
	}
	else
	{
	    SPDR = c;
	}
	SEI();//开全局中断使能
	//master_statu = "receive";
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
	uchar i = 0;
	if((flag2 & (1 << Master_RECVFlag2)) != 0)
	{
	    clr_bit(flag2, Master_RECVFlag2);
		setBEEP();
	}
}

/***************系统初始化函数 start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //屏蔽INT0~INT1的所有外部中断
    clr_bit(SFIOR,PUD); //设置输入上拉电阻有效
    DDRA=0xff;//1是输出，0是输入
    PORTA=0x00; //控制PINC与PINF通路切换
	
    DDRB=0xf7;  //1是输出，0是输入
    PORTB =0x00;
	
    DDRC=0xff; 
    PORTC =0x00; 

    DDRD  = 0x4c; //PIND0/1/4/5/7为输入
	//PORTD = 0x30;
	PORTD = 0xF3;  //外接上拉，按键带上拉
	DDRE =0xfe;  //RXD0输入，且上拉有效
	PORTE =0x01;  //TXD0输出
	
	DDRF=0xff;  //PINF全是输出
	PORTF =0x00;     
	
	DDRG =0xff;  
	PORTG =0x00;  //PING口全输出
	
	SPI_MasterInit();
	//init_TIMER0_OVF(); 
	SEI();
	
    flag1=0;
    flag2=0;
    flag3=0;
    flagerr=0;
}
/***************系统初始化函数 end ***********/
//按键处理函数===============================================
void key_scan(void)
{
    if((flag1 && (1 << keyprq_flag1)) == 0)  //如果没有按键按下
	{
	    if((PIND & (1<<key1)) == 0)  //启动测试按键
		{
		    key_now = 1;  
		}
		else if((PIND & (1<<key2)) == 0)  //急停按键
		{
		    key_now = 2;  
		}
		else
		{
		    key_now = 0;
			key_old = 0;
			key_code = 0;
		}
		if(key_now != 0)
		{
		    if(key_now != key_code)
			{
			    key_code = key_now;
				key_cnt = 0;
			}
			else
			{
			    key_cnt++;
				if(key_cnt >= KEY_COUNTER)
				{
				    set_bit(flag1, keyprq_flag1);
				}
			}
		}
	}
}
//按键处理函数===============================================
void key_process(void)
{
    if((flag1 & (1 << keyprq_flag1)) != 0)
	{
	    clr_bit(flag1, keyprq_flag1);
		if(key_code == key_old)
		{
		    ; //do nothing~
		}
		else
		{
		    key_old = key_code;
			set_bit(flag1, keyeff_flag1);  //按键有效
		}
		if((flag1 & (1 << keyeff_flag1)) != 0)
		{
		    clr_bit(flag1, keyeff_flag1);
			switch(key_old)
			{
			    case 1:  //启动测试按键按下
				{
				    for(i = 0; i < 11; i++)
		            {
		                 SPI_putchar(ok[i]);
		            }
					master_statu = "receive";
				    beeperr3();				
				}break;
				case 2:  //急停按键按下
				{
				    beeperr3();
				}break;
				default: break;
			}
		}
	}
}
/***************主函数 start *******************/
void main(void)
{
    //init_cpu();    //初始化CPU
	SPI_MasterInit();
	PORTB = 0xff;
	DDRD  = 0x4c; //PIND0/1/4/5/7为输入
	PORTD = 0x70;  //外接上拉，按键带上拉
	SEI();
	//beeperr3();
	while(1)
    {
		/*
		key_scan();
		key_process();
		delay_nms(10);//*/
		uchar temp = 0, i = 0;
		if((PIND & (1 << 4)) == 0)
		{
		    clr_bit(PORTB, 0);  //拉低从机ss引脚，启动通讯
			SPI_MasterTransmit(0xFE); //将数据通过主机MOSI传送到从机MISO
			delay_nms(500);
			set_bit(PORTB, 0);  //拉高从机ss引脚，完成通讯
			SPSR &= ~(1 << SPIF);
			delay_nms(500);
			temp = SPI_MasterReceive(); //接收从机反馈
			if(temp != 0)
			{
				beep3s();
			}//*/
		}
		if((PIND & (1 << 5)) == 0)
		{
		    set_bit(PORTB, 0);
		}
	}
}



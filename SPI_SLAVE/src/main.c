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
char *slave_statu = "receive";  //主机状态，判定发送还是接收
const uchar ok[] = {0xf3, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0a};
#define LED_BLINK() (PORTD^=0x40)

 /*SPI串行从机初始化*/
void SPI_SlaveInit(void)
{
    //MISO输出，其他为输入
	DDRB |= (1 << DDB3);
	//使能从机模式
	SPCR = (1 << SPE);
}
/*SPI从机发送一个字符*/
void SPI_SlaveTransmit(unsigned char c)
{
    //启动数据传输
	SPDR = c;
	//等待发送结束
	while(!(SPSR & (1 << SPIF)));
}
/*SPI从机接收一个字符*/
unsigned char SPI_SlaveReceive(void)
{
    SPDR = 0;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}
#define SLAVE_RX_SIZE 11  //主机接收缓冲区大小
unsigned char slave_rxBuffer[SLAVE_RX_SIZE]; //主机接收缓冲区
unsigned char slave_rxCounter = 0;  //已接收字符的个数
#define SLAVE_TX_SIZE 11
unsigned char slave_txBuffer[SLAVE_TX_SIZE];
unsigned char slave_txCounter = 0, slave_wrIndex = 0, slave_rdIndex = 0;
#pragma interrupt_handler SPI_TransEND:18
void SPI_TransEND(void)
{
    if(slave_statu == "receive")
	{
	    uchar status,data;
	    status = SPSR; //SPI状态寄存器
	    data = SPDR; //SPI数据寄存器
	    if((flag2 & (1 << Slave_RECVFlag2)) == 0)  //判断是否允许接收
	    {
			slave_rxBuffer[slave_rxCounter] = data;
			slave_rxCounter++;
			//自定义接收方式
			switch(slave_rxCounter)
			{
			    case 1: { //校验起始字节
				    if(data != 0xF3)  slave_rxCounter = 0;
				}break;
				case 2: {
				    if(data != 0xF4)  slave_rxCounter = 0;
				}break;
				case 11: {//校验结束字符
				    slave_rxCounter =0;
					if(data == 0x0A)  set_bit(flag2, Slave_RECVFlag2);
				}break;
				default:break;
			}
		}
	}
	if(slave_statu == "transmit")
	{
	    if(slave_txCounter) //队列不为空
		{
		    --slave_txCounter; //出队列
			SPDR = slave_txBuffer[slave_rdIndex];
			if(++slave_rdIndex == SLAVE_TX_SIZE)  slave_rdIndex = 0;			
		}
	}
}
//SPI发送一个字符
void SPI_putchar(unsigned char c)
{
    while(slave_txCounter == SLAVE_TX_SIZE);//等待数据填入发送缓冲区
	CLI();//关闭全局中断使能
	if(slave_txCounter || ((SPSR & (1 << SPIF)) == 0)) //缓冲区不为空
	{
	    slave_txBuffer[slave_wrIndex] = c;
		if(++slave_wrIndex == SLAVE_TX_SIZE)  slave_wrIndex = 0;
		++slave_txCounter;
	}
	else
	{
	    SPDR = c;
	}
	SEI();//开全局中断使能
	//slave_statu = "receive";
}

//定时器0初始化
void init_TIMER0_OVF(void) 
{
    TCCR0 = 0x06; //256分频
	TCNT0 = 256 - CRYSTAL/256/2*0.5;  //0.5s定时
	TIMSK |= (1<<TOIE0);  //定时器0中断使能
	SREG = 0x80;
}

uchar temp = 0;
#pragma interrupt_handler timer0_isr:17
void timer0_isr(void)
{
	//LED_BLINK();
	if((PINB & (1 << 0)) == 0) 
    {
		DDRB = 0x00;
		temp = SPI_SlaveReceive();
		if(temp != 0)
		{
		    //init_TIMER0_OVF();
		    clr_bit(PORTD, 6);
		}
    }
	
	TCNT0 = 256 - CRYSTAL/256/2*0.005;  //0.5s定时
}

/***************系统初始化函数 start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //屏蔽INT0~INT1的所有外部中断
    clr_bit(SFIOR,PUD); //设置输入上拉电阻有效
    DDRA=0xff;//1是输出，0是输入
    PORTA=0x00; //控制PINC与PINF通路切换
	
    DDRB=0xF8;  //1是输出，0是输入
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
	
	SPI_SlaveInit();
	
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
				    
					
				}break;
				case 2:  //急停按键按下
				{
				    
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
	//beeperr3();
	SPI_SlaveInit();
	PORTB = 0xfe;
	DDRD = 40;
	PORTD = 40;
	init_TIMER0_OVF();
	SEI();
	while(1)
    {	
	    /*
		key_scan();
		key_process();
		delay_nms(10);//*/
	}
}



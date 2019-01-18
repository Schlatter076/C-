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

#define TRUE 1
#define FALSE 0
//===========================================
char *isVice = "正驾";
uchar key_now = 0;
uchar key_old = 0;
uchar key_code = 0;
uchar key_cnt = 0;
uchar count = 0;
uchar vice_count = 0;

#define KEY_COUNTER 10

uchar ok[] = {0xf3, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a};
const uchar reset[] = {0xf3, 0xf4, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a};

/*============================================================*/	
/***********USART0接收中断服务函数 start**********************/ 
//USART接收缓冲区
#define RX_BUFFER_SIZE 11                  //接收缓冲区大小，可根据需要修改。
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
			switch (rx_counter)
            {
                case 1: {     // 检验起始字符
							if (data != FIRST_TEXT) rx_counter = 0;
						}break;
				    case 2: {    
							if (data != SECOND_TEXT) rx_counter = 0;
						}break;
	
                case 11: {    // 检验结束字符
							rx_counter = 0;
							if (data == END_TEXT) set_bit(flag1,Usart0_RECVFlag1);// Usart0_RecvFlag=1，表示正确接收到一个数据包
						}break;   
				default:break;
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
	if((flag1&(1<<Usart0_RECVFlag1))!=0)
	{
	   if(rx_buffer[2] == 0x01) 
		  {
		      //副驾测试
			   isVice = "副驾"; 
			   clr_bit(flag1, Usart0_RECVFlag1);
		  } 
		  else if(rx_buffer[2] == 0x10)
		  {
		      //正驾测试
			  isVice = "正驾";
			  clr_bit(flag1, Usart0_RECVFlag1);
		  } else {
		      clr_bit(flag1, Usart0_RECVFlag1);
		  } 
	}
}
//发送复位数据
void send_reset(void) 
{
    uchar i = 0;
		for(i = 0; i < TX_BUFFER_SIZE; i++)
		{
		    USART0_putchar(reset[i]);
		}
}
//发送开始按键被按下指令给上位机
void startKey_clicked(void)
{
    
    uchar index = 0;
		
		if(isVice == "正驾")
		{
		    if(count >= 19) count = 0;
			ok[9] = count;
			count++;
		}
		else if(isVice == "副驾") 
		{
		    if(vice_count >= 4) vice_count = 0;
			ok[9] = vice_count;
			vice_count++;
		}//*/
		for(index = 0; index < TX_BUFFER_SIZE; index++)
		{
		     USART0_putchar(ok[index]);
		}
}
/************USART0发送字符数组函数end****************/
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
/***************系统初始化函数 start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //屏蔽INT0~INT1的所有外部中断
    clr_bit(SFIOR,PUD); //设置输入上拉电阻有效
    DDRA=0xff;//1是输出，0是输入
    PORTA=0x00; //控制PINC与PINF通路切换
	
    DDRB=0xff;  //1是输出，0是输入
    PORTB =0x00;
	
    DDRC=0xff; 
    PORTC =0x00; 

    DDRD  = 0x4c; //PIND0/1/4/5/7为输入
	//PORTD = 0x30;
	PORTD = 0xf3;  //外接上拉，按键带上拉
	DDRE =0xfe;  //RXD0输入，且上拉有效
	PORTE =0x01;  //TXD0输出
	
	DDRF=0xff;  //PINF全是输出
	PORTF =0x00;     
	
	DDRG =0xff;  
	PORTG =0x00;  //PING口全输出
	
	//CLI();
	init_usart0();
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
				    startKey_clicked();
				}break;
				case 2:  //急停按键按下
				{
				    //startKey_clicked();
				}break;
				default: break;
			}
		}
	}
}
/***************主函数 start *******************/
void main(void)
{
   init_cpu();    //初始化CPU
	init_TIMER0_OVF();
	send_reset();
	beeperr3();
	while(1)
    {
		key_scan();
		key_process();
		delay_nms(10);
	}
}



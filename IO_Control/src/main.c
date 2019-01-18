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

#define TRUE 1
#define FALSE 0
//PIND6脉冲发送脚
#define LED_BLINK() (PORTD^=0x40)
//PB0脉冲发送
#define PAULSE() (PORTB^=0x01)
void USART0_putchar(unsigned char c);
void IO_control(void);
void uart_ok(void);
void jueyuanTest_ok(void);
void jueyuanTest_fail(void);
void stopKey_clicked(void);
void startKey_clicked(void);

//===========================================
uchar bit_count = 0;
uchar PA_var = 0;
uchar PB_var = 0;
uchar PC_var = 0;
uchar PD_var = 0;
uchar PE_var = 0;
uchar PF_var = 0;
uchar PG_var = 0;
uchar key_now = 0;
uchar key_old = 0;
uchar key_code = 0;
uchar key_cnt = 0;
uchar jueyuanOk_count = 0;
uchar jueyuanFail_count = 0;
float seconds = 0.005;
uchar count = 0;
#define JY_COUNT 3
#define KEY_COUNTER 10
#define IO_STATU 0x01
//#define READY_STATU 0x00
#define ACTION_STATU 0x02
char *uart_statu = "init";
char *jueyuan_statu = "wait";
char *product_statu = "no";

const float secs[] = {0.005, 0.0025, 0.00125, 0.00075, 0.000375};//200 400 800 1600 3200 10000

const uchar ok[] = {0xf3, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0a};
const uchar jueyuan_ok[] = {0xf3, 0xf4, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x1f, 0x02, 0x0a};
const uchar jueyuan_fail[] = {0xf3, 0xf4, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x1f, 0x02, 0x0a};
const uchar stop_key[] = {0xf3, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0a};
const uchar start_key[] = {0xf3, 0xf4, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x02, 0x0a};

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
				case 10: {    
							if (data == IO_STATU) uart_statu = "io";
							//else if(data == READY_STATU) uart_statu = "ready";
							else if(data == ACTION_STATU) uart_statu = "action";
							else uart_statu = "init";
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
//定时中断函数(自动检测用) 
#pragma interrupt_handler timer1_count_isr:15  
void timer1_count_isr(void) 
{ 
	PAULSE();
	TCNT1=65536-CRYSTAL/8/2*0.0005;
}
//定时器1初始化
void init_TIMER1_OVF(void)
{
	TCCR1B=0x02;  //8分频
	TCNT1=65536-CRYSTAL/8/2*0.5;//0.5s定时
	TIMSK|=(1<<TOIE1);  //定时器使能
	SREG=0x80;      //全局使能中断
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
	LED_BLINK();
	if((flag1&(1<<Usart0_RECVFlag1))!=0)
	{
	   IO_control();
	}
	//防止数据校验位出错导致占位
	if(uart_statu == "init")
	{
	    clr_bit(flag1, Usart0_RECVFlag1);
	}
	if((PIND & (1 << 7)) == 0)
	{
	    product_statu = "have";
	}
	else
	{
	    product_statu = "no";
	}
	if((PIND & (1 << 0)) == 0)
	{
	    jueyuan_statu = "ok";
		jueyuanTest_ok();
	}
	else if((PIND & (1 << 1)) == 0)
	{
	    jueyuan_statu = "fail";
		jueyuanTest_fail();
	}
	else
	{
	    jueyuan_statu = "wait";
		jueyuanOk_count = 0;
		jueyuanFail_count = 0;
	}
}
//根据上位机指令配置端口输出
void IO_control(void) 
{
    if(uart_statu == "io")
	{
	    uart_statu = "init";
		clr_bit(flag1, Usart0_RECVFlag1);
		PA_var = rx_buffer[2];
		PB_var = rx_buffer[3];
		PC_var = rx_buffer[4];
		PD_var = rx_buffer[5];
		PE_var = rx_buffer[6];
		PF_var = rx_buffer[7];
		PG_var = rx_buffer[8];
		//输出控制开始
		for(bit_count = 0; bit_count < 8; bit_count++)
		{
			if((PA_var & (1 << bit_count)) != 0)
			{
			    set_bit(PORTA, bit_count);
			}
			else
			{
			    clr_bit(PORTA, bit_count);
			}
			if((PB_var & (1 << bit_count)) != 0)
			{
			    if(bit_count != 0)
				    set_bit(PORTB, bit_count);
			}
			else
			{
			    if(bit_count != 0)
				    clr_bit(PORTB, bit_count);
			}
			if((PC_var & (1 << bit_count)) != 0)
			{
			    set_bit(PORTC, bit_count);
			}
			else
			{
			    clr_bit(PORTC, bit_count);
			}
			
			if((PD_var & (1 << bit_count)) != 0)
			{
			    if(bit_count == 2 || bit_count == 3)
				{
				    set_bit(PORTD, bit_count);
				} 
			}
			else
			{
			    if(bit_count == 2 || bit_count == 3)
				{
				    clr_bit(PORTD, bit_count);
				} 
			}
			if((PE_var & (1 << bit_count)) != 0)
			{
			    if(bit_count != 0 && bit_count != 1)
				{
				    set_bit(PORTE, bit_count);
				}
			}
			else
			{
			    if(bit_count != 0 && bit_count != 1)
				{
				    clr_bit(PORTE, bit_count);
				}
			}//*/
			if((PF_var & (1 << bit_count)) != 0)
			{
			    set_bit(PORTF, bit_count);
			}
			else
			{
			    clr_bit(PORTF, bit_count);
			}
			if((PG_var & (1 << bit_count)) != 0 && bit_count < 5)
			{
			    set_bit(PORTG, bit_count);
			}
			else
			{
			    if(bit_count < 5)
				{
				    clr_bit(PORTG, bit_count);
				}
			}
		}//输出控制结束
		delay_nms(100);
		uart_ok();  //将数据反馈给上位机
	}
	else
	    return;
}
//发送绝缘测试ok指令给上位机
void jueyuanTest_ok(void)
{
    uchar i = 0;
	if(jueyuan_statu == "ok" && jueyuanOk_count < JY_COUNT)
	{
	    jueyuanOk_count++;
		for(i = 0; i < 11; i++)
	    {
	        USART0_putchar(jueyuan_ok[i]);
	    }
	}
	
}
//发送绝缘测试fail指令给上位机
void jueyuanTest_fail(void)
{
    uchar i = 0;
	if(jueyuan_statu == "fail" && jueyuanFail_count < JY_COUNT)
	{
	    jueyuanFail_count++;
		for(i = 0; i < 11; i++)
	    {
	        USART0_putchar(jueyuan_fail[i]);
	    }
	}
	
}
//发送IO口配置成功指令给上位机
void uart_ok(void) 
{
    uchar i = 0;
	for(i = 0; i < 11; i++) 
    {
	    USART0_putchar(ok[i]);
	}
}
//发送急停按键被按下指令给上位机
void stopKey_clicked(void)
{
    uchar i = 0;
	for(i = 0; i < 11; i++)
	{
	    USART0_putchar(stop_key[i]);
	}
}
//发送开始按键被按下指令给上位机
void startKey_clicked(void)
{
    uchar i = 0;
	if(product_statu == "have")
	{
	    for(i = 0; i < 11; i++)
	    {
	        USART0_putchar(start_key[i]);
	    }
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
				    //startKey_clicked();
					for(count = 0; count < sizeof(secs); count++)
					{
					    seconds = secs[count];
						//delay_nms(200);
					}
				}break;
				case 2:  //急停按键按下
				{
				    stopKey_clicked();
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
	init_TIMER1_OVF();
	//beeperr3();
	while(1)
    {
		key_scan();
		key_process();
		delay_nms(10);
	}
}



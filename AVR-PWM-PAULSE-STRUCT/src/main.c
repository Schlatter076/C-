/******************************************/
/*           广州旭东阪田电子有限公司     */
/*Project:     相位可变PWM波形发生        */
/*Guest:                                  */
/*Name:             main.c                */
/*Mcu chip:         Atmega64              */
/*Main clock:       外部晶体11.0592MHz    */
/*Rtc clock:                              */
/*Author:                                 */
/*Create date:      2019.07.27            */
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
#define ut8 unsigned char
#define ut16 unsigned int

ut16 T_Count = 0;
/***************变量定义 start**************/
//key===========================================================================
uchar temp_jizhong = 0;
uchar key_temp = 0;
uchar key_now = 0;
uchar key_code = 0;
uchar key_old = 0;
uchar key_cnt = 0;
uchar ver_select = 0;
uchar key_counter = 5;
uchar num = 0;

ut16 duty = 0;

uchar pau_ctrl[] = {0xf3, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0a};

//===============================串口参数定义Start===================================
//串口0接收缓冲区大小
#define RX_BUFFER_SIZE 11
//串口0发送缓冲区大小
#define TX_BUFFER_SIZE 11
//串口1接收缓冲区大小
#define RX_BUFFER_SIZE_1 11
//串口1发送缓冲区大小
#define TX_BUFFER_SIZE_1 11
//串口0参数结构体
struct
{
		unsigned char rx_buffer[RX_BUFFER_SIZE]; //接收缓冲区
		unsigned char rx_counter; //接收缓冲区已读个数
		unsigned char tx_buffer[TX_BUFFER_SIZE]; //发送缓冲区
		unsigned char tx_wr_index;  //发送缓冲区写入计数
		unsigned char tx_rd_index; //发送缓冲区读出计数
		unsigned char tx_counter; //发送缓冲区队列计数
}UART0;
//串口1参数结构体
struct
{
		unsigned char rx_buffer[RX_BUFFER_SIZE_1]; //接收缓冲区
		unsigned char rx_counter; //接收缓冲区已读个数
		unsigned char tx_buffer[TX_BUFFER_SIZE_1]; //发送缓冲区
		unsigned char tx_wr_index;  //发送缓冲区写入计数
		unsigned char tx_rd_index; //发送缓冲区读出计数
		unsigned char tx_counter; //发送缓冲区队列计数
}UART1;
//===============================串口参数定义End=====================================
/***********USART0接收中断服务函数 start**********************/
//定义一个标志位Usart0_RECVFlag1:=1表示串口0接收到了一个完整的数据包
//在port.h中定义
#pragma interrupt_handler usart0_rxc_isr:19  //接收中断服务程序
void usart0_rxc_isr(void)
{
    uchar status, data;
    status = UCSR0A;
    data = UDR0;
    if((flag1 & (1 << Usart0_RECVFlag1)) == 0) //判断是否允许接收一个新的数据包
    {
        if ((status & (USART0_FRAMING_ERROR | USART0_PARITY_ERROR | USART0_DATA_OVERRUN)) == 0)
        {
            UART0.rx_buffer[UART0.rx_counter] = data;
            UART0.rx_counter++;
            switch (UART0.rx_counter)
            {
            case 1:       // 检验起始字符
            {
                if (data != 0xf3) UART0.rx_counter = 0;

            }
            break;
            case 2:
            {
                if (data != 0xf4) UART0.rx_counter = 0;

            }
            break;
            case 11:      // 检验结束字符
            {
                UART0.rx_counter = 0;
                if(data == 0x0a)
                    set_bit(flag1, Usart0_RECVFlag1); // Usart0_RecvFlag=1，表示正确接收到一个数据包
            }
            break;
            default:
                break;
            }
        }
    }
}
/***************USART0接收中断服务函数 end**********************/
/*============================================================*/
/*============================================================*/
/***************USART0发送中断服务函数 start********************/
#pragma interrupt_handler usart0_txc_isr:21  //发送中断服务程序
void usart0_txc_isr(void)
{
    if (UART0.tx_counter)//队列不为空
    {
        --UART0.tx_counter;//出队列
        UDR0 = UART0.tx_buffer[UART0.tx_rd_index];
        if (++UART0.tx_rd_index == TX_BUFFER_SIZE) UART0.tx_rd_index = 0;
    }
}
/***********USART0发送中断服务函数 end**********************/

/*============================================================*/
/***********USART0发送一个字符函数 start**********************/
void USART0_putchar(unsigned char c)
{
    while (UART0.tx_counter == TX_BUFFER_SIZE);
    CLI();//#asm("cli")关闭全局中断允许
    if (UART0.tx_counter || ((UCSR0A & USART0_DATA_REGISTER_EMPTY) == 0)) //发送缓冲器不为空
    {
        UART0.tx_buffer[UART0.tx_wr_index] = c; //数据进入队列
        if (++UART0.tx_wr_index == TX_BUFFER_SIZE) UART0.tx_wr_index = 0; //队列已满
        ++UART0.tx_counter;
    }
    else
        UDR0 = c;
    SEI(); //#asm("sei")打开全局中断允许
}
/***********USART0发送服务函数 end**********************/
/*============================================================*/
/**
 * 串口0发送一次指令
 * @buff 指令数组，长度为TX_BUFFER_SIZE
 */
void UART0_send(unsigned char buff[])
{
    unsigned char buff_cnt;
    for(buff_cnt = 0; buff_cnt < TX_BUFFER_SIZE; buff_cnt++)
    {
        USART0_putchar(buff[buff_cnt]);
    }
}
/**
 * 串口0发送多次指令
 * @buff 指令数组，长度为TX_BUFFER_SIZE_1
 * @times 发送的次数
 * @dealys 每次发送的时间间隔(单位ms毫秒)
 */
void UART0_send_multiple(unsigned char buff[], unsigned char times, unsigned int delays)
{
    uchar i = 0;
    do
    {
        if(i++ >= times)
        {
            i = 0;
            break; //发送超时，结束循环
        }
        UART0_send(buff);
        delay_nms(delays);
    }
    while((flag1 & (1 << Usart0_RECVFlag1)) == 0);
}
/*============================================================*/
/***************USART0初始化函数 start*************************/
void init_usart0(void)
{
    UCSR0B = 0x00;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //异步，8位数据，无奇偶校验，一个停止位，无倍速
    UBRR0L = BAUD_L;                        //设定波特率
    UBRR0H = BAUD_H;
    UCSR0A = 0x00;
    UCSR0B = (1 << RXCIE0) | (1 << TXCIE0) | (1 << RXEN0) | (1 << TXEN0); //0XD8  接收、发送使能， 开中断使能
}
/***************USART0初始化函数 end***************************/
/***************USART1初始化函数 start*************************/
void init_usart1(void)
{
    UCSR1B = 0x00;
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);  //异步，8位数据，无奇偶校验，一个停止位，无倍速
    UBRR1L = BAUD_L;                        //设定波特率
    UBRR1H = BAUD_H;
    UCSR1A = 0x00;
    UCSR1B = (1 << RXCIE1) | (1 << TXCIE1) | (1 << RXEN1) | (1 << TXEN1); //0XD8  接收、发送使能， 开中断使能
}
/***************USART1初始化函数 end***************************/
/***************USART1发送中断服务函数 start********************/
#pragma interrupt_handler usart1_txc_isr:33  //发送中断服务程序
void usart1_txc_isr(void)
{
    if (UART1.tx_counter)//队列不为空
    {
        --UART1.tx_counter;//出队列
        UDR1 = UART1.tx_buffer[UART1.tx_rd_index];
        if (++UART1.tx_rd_index == TX_BUFFER_SIZE_1) UART1.tx_rd_index = 0;
    }
}
/***********USART1发送中断服务函数 end**********************/
/***********USART1发送一个字符函数 start**********************/
void USART1_putchar(unsigned char c)
{
    while (UART1.tx_counter == TX_BUFFER_SIZE);
    CLI();//#asm("cli")关闭全局中断允许
    if (UART1.tx_counter || ((UCSR1A & USART1_DATA_REGISTER_EMPTY) == 0)) //发送缓冲器不为空
    {
        UART1.tx_buffer[UART1.tx_wr_index] = c; //数据进入队列
        if (++UART1.tx_wr_index == TX_BUFFER_SIZE_1) UART1.tx_wr_index = 0; //队列已满
        ++UART1.tx_counter;
    }
    else
        UDR1 = c;
    SEI(); //#asm("sei")打开全局中断允许
}
/***********USART1发送服务函数 end**********************/
/***********USART1接收中断服务函数 start**********************/
//定义一个标志位Usart1_RECVFlag1:=1表示串口1接收到了一个完整的数据包
//在port.h中定义

#pragma interrupt_handler usart1_rxc_isr:31  //接收中断服务程序
void usart1_rxc_isr(void)
{
    uchar status, data;
    status = UCSR1A;
    data = UDR1;
    if((flag1 & (1 << Usart1_RECVFlag1)) == 0) //判断是否允许接收一个新的数据包
    {
        if ((status & (USART1_FRAMING_ERROR | USART1_PARITY_ERROR | USART1_DATA_OVERRUN)) == 0)
        {
            UART1.rx_buffer[UART1.rx_counter] = data;
            UART1.rx_counter++;
            switch (UART1.rx_counter)
            {
            case 1:       // 检验起始字符
            {
                if (data != 0xf3) UART1.rx_counter = 0;

            }
            break;
            case 2:
            {
                if (data != 0xf4) UART1.rx_counter = 0;

            }
            break;
            case 11:      // 检验结束字符
            {
                UART1.rx_counter = 0;
                if(data == 0x0a)
                    set_bit(flag1, Usart1_RECVFlag1); // Usart1_RecvFlag=1，表示正确接收到一个数据包
            }
            break;
            default:
                break;
            }
        }
    }
}
/***************USART1接收中断服务函数 end**********************/
/**
 * 串口1发送一次指令
 * @buff 指令数组，长度为TX_BUFFER_SIZE_1
 */
void UART1_send(unsigned char buff[])
{
    unsigned char buff_cnt;
    for(buff_cnt = 0; buff_cnt < TX_BUFFER_SIZE_1; buff_cnt++)
    {
        USART1_putchar(buff[buff_cnt]);
    }
}
/**
 * 串口1发送多次指令
 * @buff 指令数组，长度为TX_BUFFER_SIZE_1
 * @times 发送的次数
 * @dealys 每次发送的时间间隔(单位ms毫秒)
 */
void UART1_send_multiple(unsigned char buff[], unsigned char times, unsigned int delays)
{
    uchar i = 0;
    do
    {
        if(i++ >= times)
        {
            i = 0;
            break; //发送超时，结束循环
        }
        UART1_send(buff);
        delay_nms(delays);
    }
    while((flag1 & (1 << Usart1_RECVFlag1)) == 0);
}
/*============================================================*/
/*
void initTC1_PWM(void)
{
    TCCR1A = 0x54; //通道A/B/C比较输出模式，8分频
    TCCR1B = 0x0a; //CTC比较匹配时电平取反
		TIMSK |= (1 << OCIE1A); //输出比较A匹配中断使能
		SEI(); //全局中断使能
		OCR1A = CRYSTAL / 8 / 2 / 50; //比较输出附初值(20HZ)
}//*/
/**
 * 初始化TC1输出PWM
 */
void initTC1_PWM(void)
{
    TCCR1A = 0xa8; //升序匹配清零，降序匹配置位(ICR1模式)
    TCCR1B = 0x12; //OC1A OC1B OC1C比较输出相频修正PWM 8分频
    TIMSK |= (1 << OCIE1A); //输出比较A匹配中断使能
    ICR1 = ICR1 = CRYSTAL / 8 / 2 / 20; //初始20HZ
    OCR1A = OCR1B = OCR1C = ICR1 / 2; //设置占空比为50%
    SEI(); //全局中断使能
}
/**
 * 设置TC1输出脉冲频率
 * @hz 赫兹
 */
void setTC1_frequency(unsigned int hz)
{
    unsigned char sreg;
    sreg = SREG;
    CLI();
    ICR1 = CRYSTAL / 8 / 2 / hz;
    OCR1A = OCR1B = OCR1C = ICR1 / 2;
    SREG = sreg;
}
/**
 * 允许TC1输出PWM波
 * @TCCR1A_bit 输出通路选择(7->通路A 5->通路B 3->通路C)
 */
void allowTC1_PWM(unsigned char TCCR1A_bit)
{
    unsigned char sreg;
    sreg = SREG;
    CLI();
    TCCR1A |= (1 << TCCR1A_bit);
    SREG = sreg;
}
/**
 * 禁止TC1输出PWM波
 * @TCCR1A_bit 输出通路选择(7->通路A 5->通路B 3->通路C)
 */
void prohibitedTC1_PWM(unsigned char TCCR1A_bit)
{
    unsigned char sreg;
    sreg = SREG;
    CLI();
    TCCR1A &= ~(1 << TCCR1A_bit);
    SREG = sreg;
}
//TC1输出匹配A中断服务函数
#pragma interrupt_handler TC1_compA_ISR:13
void TC1_compA_ISR(void)
{
    PORTD ^= 0x40;
    PORTB ^= 0x10;
    if((flag1 & (1 << Usart1_RECVFlag1)) != 0)
    {
        clr_bit(flag1, Usart1_RECVFlag1);
        setTC1_frequency((UART1.rx_buffer[2] << 8) | UART1.rx_buffer[3]);
			UART1_send(pau_ctrl);
    }
		if((flag1 & (1 << Usart0_RECVFlag1)) != 0)
    {
        clr_bit(flag1, Usart0_RECVFlag1);
			delay_nms(2000);
			prohibitedTC1_PWM(COM1B1);
    }
}
/***************系统初始化函数 start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //屏蔽INT0~INT1的所有外部中断
    clr_bit(SFIOR, PUD); //设置输入上拉电阻有效
    DDRA = 0xff; //1是输出，0是输入
    PORTA = 0x00; //LCD的数据口

    DDRB = 0xff; //1是输出，0是输入
    PORTB = 0xff; //PB5/6/7脉冲输出口，带上拉

    DDRC = 0xff;
    PORTC = 0x00;

    DDRD  = 0x49;   //RXD1输入，且上拉有效
    PORTD = 0xf4;   //TXD1输出。

    DDRE = 0x0e;   //RXD0输入，且上拉有效
    PORTE = 0x01;  //TXD0输出

    DDRF = 0xff;
    PORTF = 0x00;

    DDRG = 0xf0;
    PORTG = 0x00; //PG3为产品感应器检测口//PG4继电器

    init_usart0();
    init_usart1();
    initTC1_PWM();
    SEI();

    flag1 = 0;
    flag2 = 0;
    flag3 = 0;
    flagerr = 0;
}
/***************系统初始化函数 end ***********/
/*************按键检测函数 start***************/
void key_scan(void)
{
    if((flag1 & (1 << keyprq_flag1)) == 0)
    {
        if((PIND & (1 << key1)) == 0) key_now = 1;   //auto按键
        else if((PIND & (1 << key2)) == 0) key_now = 2; //men按键
        else
        {
            key_now = 0;
            key_code = 0;
            key_old = 0;
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
                if(key_cnt >= key_counter) //平常状况下是10次,但是测试VR1时则是1次
                {
                    key_cnt = 0;
                    set_bit(flag1, keyprq_flag1);
                }
            }
        }
    }
}
/**************按键检测函数 end****************/
void paulse_change(void)
{
    uchar i;
    duty += 200;
		pau_ctrl[2] = (duty >> 8) & 0xff;
		pau_ctrl[3] = duty & 0xff;
    for(i = 0; i < TX_BUFFER_SIZE; i++)
    {
        USART0_putchar(pau_ctrl[i]);
    }
}
/**************按键处理函数 start**************/
void key_process(void)
{
    if((flag1 & (1 << keyprq_flag1)) != 0)
    {
        clr_bit(flag1, keyprq_flag1);
        if(key_code == key_old)
        {
            ;
        }
        else
        {
            key_old = key_code;
            set_bit(flag1, keyeff_flag1);
        }
        if((flag1 & (1 << keyeff_flag1)) != 0)
        {
            clr_bit(flag1, keyeff_flag1);
            switch(key_old)
            {
            case 1:  //auto按键
            {
                paulse_change();
            }
            break;
            case 2:  //men按键(机种选择)
            {
				 	 //prohibitedTC1_PWM(COM1A1);
					 //prohibitedTC1_PWM(COM1B1);
					 //prohibitedTC1_PWM(COM1C1);
            }
            break;
            default:
                break;
            }
        }
    }
}
/**************按键处理函数 end****************/

/***************主函数 start *******************/
void main(void)
{
    init_cpu();    //初始化CPU
    while(1)
    {
        key_scan();
        key_process();
        delay_nms(10);
    }
}
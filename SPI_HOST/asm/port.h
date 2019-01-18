
/******************************************/
/*           广州旭东阪田电子有限公司     */
/*Project:      FCT测试主板               */
/*Guest:                                  */
/*Name:             port.h                */
/*Mcu chip:         Atmega64              */
/*Main clock:       外部晶体11.0592MHz    */
/*Rtc clock:                              */
/*Author:           Jack.Fu               */
/*Create date:      2008.11.20            */
/*Design date:                            */
/*Complete date:                          */
/******************************************/

#ifndef _PORT_H
#define _PORT_H

//extern unsigned char read_flash(unsigned int add);

/*================================================*/
/************定义MCU I/O口 start*******************/
//1是输出，0是输入
//PORTA  都是输出
#define LCD_data PORTA

//PORTB  都是输出
#define relay1   0
#define relay2   1
#define relay3   2
#define relay4   3
#define relay5   4
#define relay6   5
#define relay7   6
#define relay8   7  
#define relay_1_8_PORT PORTB
#define relay_9_16_PORT PORTC

//PORTC  都是输出
#define PC0    0
#define PC1    1
#define PC2    2
#define PC3    3
#define PC4    4
#define PC5    5
#define PC6    6
#define PC7    7 

//PORTD
#define K19  0  //继电器
#define relay26  1  //两个继电器
#define rxd1     2  //RXD输入，且上拉有效      ;USART1串口用于接PC，公座
#define txd1     3  //TXD输出                  串口USART1的数据口和时钟口
#define key1 4  //自动测试开始的按键,输入
#define key2 5  //急停按键，输入
#define bell 6  //蜂鸣器控制，pnp管，低电平有效。  输出
#define relay27 7  //继电器
 
//PORTE
#define rxd0     0  //RXD输入，且上拉有效      ;USART0串口用于接FLUKE万用表，母座
#define txd0     1  //TXD输出                  串口USART0的数据口和时钟口
#define LCD_RS   2
#define LCD_EN   3  //LCD的控制口，输出
#define relay28  4
#define relay29  5
#define relay30  6
#define relay31  7  //4个继电器控制，输出

//PORTF
#define K8 0
#define K7 1
#define K6 2
#define K5 3
#define K4 4
#define K3 5
#define K2 6
#define K1 7

//PORTG
#define relay32 0
#define relay33 1  
#define relay34 2  
#define LED2 3
#define K20 4  

/************定义MCU I/O口 end*********************/
/*================================================*/

/*================================================*/
/************定义MCU Flag标志位 start**************/
#pragma global_register flag1:20 flag2:21 flag3:22 flagerr:23
unsigned char flag1;
#define keyprq_flag1     0	//有键按下标志位
#define keyeff_flag1     1	//按键有效标志位
#define Usart0_RECVFlag1 2 //=1表示串口0接收到了一个完整的数据包
#define Usart1_RECVFlag1 3 //=1表示串口1接收到了一个完整的数据包
#define error_flag1      4   //通信错误标志位
#define AD0_RECVFlag1    5 //=1表示ADC0接收到了一个完整的数据包
#define time_goFlag1     6
#define recv_okFlag1     7

unsigned char flag2;
#define Master_RECVFlag2 0 //SPI主机接收完整数据包
#define Slave_RECVFlag2 1 //SPI从机接收完整数据包

unsigned char flag3;
unsigned char flagerr;


/************定义MCU Flag标志位 end****************/
/*================================================*/

#endif


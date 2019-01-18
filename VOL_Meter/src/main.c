/******************************************/
/*           ����������������޹�˾     */
/*Project:     �����Զ����FCT����        */
/*Guest:                                  */
/*Name:             main.c                */
/*Mcu chip:         Atmega64              */
/*Main clock:       �ⲿ����11.0592MHz    */
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
/***********USART0�����жϷ����� start**********************/ 
//USART���ջ�����
#define RX_BUFFER_SIZE 30                  //���ջ�������С���ɸ�����Ҫ�޸ġ�
unsigned char rx_buffer[RX_BUFFER_SIZE];   //������ջ�����
unsigned char rx_counter=0;                //����rx_counterΪ����ڶ����е��ѽ��յ��ַ�������

//����һ����־λUsart0_RECVFlag1:=1��ʾ����0���յ���һ�����������ݰ�
//��port.h�ж���

#pragma interrupt_handler usart0_rxc_isr:19  //�����жϷ������
void usart0_rxc_isr(void)
{
    uchar status,data;
    status=UCSR0A;
    data=UDR0;
	if((flag1&(1<<Usart0_RECVFlag1))==0)   //�ж��Ƿ��������һ���µ����ݰ�
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
/***************USART0�����жϷ����� end**********************/ 
/*============================================================*/
/*============================================================*/	
/***************USART0�����жϷ����� start********************/ 
#define TX_BUFFER_SIZE 11    
unsigned char tx_buffer[TX_BUFFER_SIZE];  
unsigned char tx_wr_index=0,tx_rd_index=0,tx_counter=0;

#pragma interrupt_handler usart0_txc_isr:21  //�����жϷ������
void usart0_txc_isr(void)
{
    if (tx_counter)//���в�Ϊ��
    {
        --tx_counter;//������
        UDR0=tx_buffer[tx_rd_index];
        if (++tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
    }
}
/***********USART0�����жϷ����� end**********************/ 

/*============================================================*/
/***********USART0����һ���ַ����� start**********************/ 
void USART0_putchar(unsigned char c)
{
    while (tx_counter == TX_BUFFER_SIZE);
    CLI();//#asm("cli")�ر�ȫ���ж�����
    if (tx_counter || ((UCSR0A & USART0_DATA_REGISTER_EMPTY)==0))//���ͻ�������Ϊ��
    {
        tx_buffer[tx_wr_index]=c;//���ݽ������
        if (++tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;//��������
        ++tx_counter;
    }
    else
        UDR0=c;
    SEI(); //#asm("sei")��ȫ���ж�����
}
/***********USART0���ͷ����� end**********************/ 
/***************USART0��ʼ������ start*************************/ 
void init_usart0(void)
{
    UCSR0B = 0x00; 
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);        //�첽��8λ���ݣ�����żУ�飬һ��ֹͣλ���ޱ��� 
    UBRR0L = BAUD_L;                        //�趨������ 
    UBRR0H = BAUD_H; 
    UCSR0A = 0x00; 
    UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0); //0XD8  ���ա�����ʹ�ܣ� ���ж�ʹ��
}
/***************USART0��ʼ������ end***************************/
#pragma interrupt_handler timer1_count_isr:15  
void timer1_count_isr(void) 
{ 
	for(cmd_count = 0; cmd_count < 8; cmd_count++)
	{
	    USART0_putchar(vol_1[cmd_count]);
	}
}
//��ʱ����ʼ��
void init_TIMER1_OVF(void)
{
	TCCR1B=0x02;  //8��Ƶ
	TCNT1=65536-CRYSTAL/8*1.0;//0.5s��ʱ
	TIMSK|=(1<<TOIE1);  //��ʱ��ʹ��
	SREG=0x80;      //ȫ��ʹ���ж�
}

/***************ϵͳ��ʼ������ start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //����INT0~INT1�������ⲿ�ж�
    clr_bit(SFIOR,PUD); //������������������Ч
    DDRA=0xff;//1�������0������
    PORTA=0x00; //LCD�����ݿ�
	
    DDRB=0x20;  //1�������0������
    PORTB =0xff;//PB5����
	
    DDRC=0xff; 
    PORTC =0x00;

    DDRD  = 0x41;   //RXD1���룬��������Ч  
	PORTD = 0xf4;   //TXD1�����
	
	DDRE =0x0e;    //RXD0���룬��������Ч
	PORTE =0x01;   //TXD0���
	
	DDRF=0xff;
	PORTF =0x00;     
	
	DDRG =0xf0;  
	PORTG =0x00;  //PG3Ϊ��Ʒ��Ӧ������//PG4�̵���
	
	init_usart0();

    flag1=0;
    flag2=0;
    flag3=0;
    flagerr=0;
}
/***************ϵͳ��ʼ������ end ***********/

/***************������ start *******************/
void main(void)
{
    init_cpu();    //��ʼ��CPU
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



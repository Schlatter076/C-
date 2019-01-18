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
//���λ��
Pozition left_motor = {0, 0, 0, {0}};

const uchar str[] = "Fuck you!";
const uchar str_1[] = "Failed!";
const uchar start[MY_SIZE] = {0x60, 0x00, 0x81, 0x00, 0x68, 0x00, 0x84, 0x00, 0x79, 0x00, 0x80, 0x00, 0x67, 0x00, 0x62};
const uchar stop[MY_SIZE] = {0x60, 0x00, 0x83, 0x00, 0x50, 0x00, 0x67, 0x00, 0x83, 0x00, 0x87, 0x00, 0x67, 0x00, 0x62};
const uchar R_measure[MY_SIZE] = {0x60, 0x00, 0x67, 0x00, 0x83, 0x00, 0x68, 0x00, 0x90, 0x00, 0x83, 0x00, 0x49, 0x00, 0x62};
const uchar over[MY_SIZE] = {0x60, 0x00, 0x67, 0x00, 0x83, 0x00, 0x68, 0x00, 0x90, 0x00, 0x49, 0x00, 0x83, 0x00, 0x62};
const uchar ask[MY_SIZE] = {0x60, 0x00, 0x83, 0x00, 0x67, 0x00, 0x68, 0x00, 0x90, 0x00, 0x49, 0x00, 0x83, 0x00, 0x62};
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
void EEPROM_save(const uchar cmd[MY_SIZE])
{
    uint cmd_count = 0;
	for(cmd_count = 0; cmd_count < MY_SIZE; cmd_count++)
	{
		EEPROMwrite(cmd_count, ask[cmd_count]);
	}
}
/************USART0�����ַ����麯��start****************/
void sendCMD(const uchar cmd[MY_SIZE]) 
{
	uint cmd_count = 0;
	for(cmd_count = 0; cmd_count < MY_SIZE; cmd_count++)
	{
		USART0_putchar(cmd[cmd_count]);
	}
}
/************USART0�����ַ����麯��end****************/
/************USART0�����ַ����麯��start****************/
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
/************USART0�����ַ����麯��end****************/
/***************USART01��ʼ������ start*************************/ 
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

/*********�������ݺ��� start *******************/
void fetch_data(void)			//��������	
{
	unsigned char n =0;
	uint i = 0;
	unsigned char num = 0;
	uchar *pstr1;
	uchar *pstr2;
	uchar *pstr3;
	pstr1 = rx_buffer;					//pstr1ָ���ַ���ͷ��
	pstr3=strchr(rx_buffer,'.');        //ָ��С����
	if(pstr3==NULL) set_bit(flag1,error_flag1);  //=�գ���ʾû���ҵ�С����
	pstr2 = strchr(rx_buffer, ' ');	    //pstr2ָ��ո�
	if(pstr2 == NULL) set_bit(flag1, error_flag1);//�Ҳ����ո��ʾͨ���д���
	num = pstr2 - pstr1;				//�����ַ�������
	
	
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
	//USART0_putchar(0x0d); //����һ���س�
	//USART0_putchar(0x0a); //����һ������
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
/*********�������ݺ��� end***********************/


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



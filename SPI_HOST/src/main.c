/******************************************/
/*           ����������������޹�˾     */
/*Project:          SPI����               */
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

#define TRUE 1
#define FALSE 0

//key
uchar key_now = 0;
uchar key_old = 0;
uchar key_code = 0;
uchar key_cnt = 0;
#define KEY_COUNTER 10
//SPI
char *master_statu = "transmit";  //����״̬���ж����ͻ��ǽ���
const uchar ok[] = {0xf3, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0a};
uchar i = 0;

 /*SPI����������ʼ��*/
void SPI_MasterInit(void)
{
    //MOSI��SCK���������Ϊ����
	DDRB = (1 << DDB2)|(1 << DDB1);
	//ʹ������ģʽ������ʱ������fsc/16
	SPCR = (1 << SPE)|(1 << MSTR)|(1 << SPR0);
}
/*SPI��������һ���ַ�*/
void SPI_MasterTransmit(unsigned char c)
{
    //�������ݴ���
	SPDR = c;
	//�ȴ����ͽ���
	while(!(SPSR & (1 << SPIF)));
}
/*SPI��������һ���ַ�*/
unsigned char SPI_MasterReceive(void)
{
    SPDR = 0;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}
#define MASTER_RX_SIZE 11  //�������ջ�������С
unsigned char master_rxBuffer[MASTER_RX_SIZE]; //�������ջ�����
unsigned char master_rxCounter = 0;  //�ѽ����ַ��ĸ���
#define MASTER_TX_SIZE 11
unsigned char master_txBuffer[MASTER_TX_SIZE];
unsigned char master_txCounter = 0, master_wrIndex = 0, master_rdIndex = 0;
#pragma interrupt_handler SPI_TransEND:18
void SPI_TransEND(void)
{
    if(master_statu == "receive")
	{
	    uchar status,data;
	    status = SPSR; //SPI״̬�Ĵ���
	    data = SPDR; //SPI���ݼĴ���
	    if((flag2 & (1 << Master_RECVFlag2)) == 0)  //�ж��Ƿ��������
	    {
			master_rxBuffer[master_rxCounter] = data;
			master_rxCounter++;
			//�Զ�����շ�ʽ
			switch(master_rxCounter)
			{
			    case 1: { //У����ʼ�ֽ�
				    if(data != 0xF3)  master_rxCounter = 0;
				}break;
				case 2: {
				    if(data != 0xF4)  master_rxCounter = 0;
				}break;
				case 11: {//У������ַ�
				    master_rxCounter =0;
					if(data == 0x0A)  set_bit(flag2, Master_RECVFlag2);
				}break;
				default:break;
			}
		}
	}
	if(master_statu == "transmit")
	{
	    if(master_txCounter) //���в�Ϊ��
		{
		    --master_txCounter; //������
			SPDR = master_txBuffer[master_rdIndex];
			if(++master_rdIndex == MASTER_TX_SIZE)  master_rdIndex = 0;			
		}
	}
}
//SPI����һ���ַ�
void SPI_putchar(unsigned char c)
{
    while(master_txCounter == MASTER_TX_SIZE);//�ȴ��������뷢�ͻ�����
	CLI();//�ر�ȫ���ж�ʹ��
	if(master_txCounter || ((SPSR & (1 << SPIF)) == 0)) //��������Ϊ��
	{
	    master_txBuffer[master_wrIndex] = c;
		if(++master_wrIndex == MASTER_TX_SIZE)  master_wrIndex = 0;
		++master_txCounter;
	}
	else
	{
	    SPDR = c;
	}
	SEI();//��ȫ���ж�ʹ��
	//master_statu = "receive";
}

//��ʱ��0��ʼ��
void init_TIMER0_OVF(void) 
{
    TCCR0 = 0x06; //256��Ƶ
	TCNT0 = 256 - CRYSTAL/256/2*0.5;  //0.5s��ʱ
	TIMSK |= (1<<TOIE0);  //��ʱ��0�ж�ʹ��
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

/***************ϵͳ��ʼ������ start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //����INT0~INT1�������ⲿ�ж�
    clr_bit(SFIOR,PUD); //������������������Ч
    DDRA=0xff;//1�������0������
    PORTA=0x00; //����PINC��PINFͨ·�л�
	
    DDRB=0xf7;  //1�������0������
    PORTB =0x00;
	
    DDRC=0xff; 
    PORTC =0x00; 

    DDRD  = 0x4c; //PIND0/1/4/5/7Ϊ����
	//PORTD = 0x30;
	PORTD = 0xF3;  //�������������������
	DDRE =0xfe;  //RXD0���룬��������Ч
	PORTE =0x01;  //TXD0���
	
	DDRF=0xff;  //PINFȫ�����
	PORTF =0x00;     
	
	DDRG =0xff;  
	PORTG =0x00;  //PING��ȫ���
	
	SPI_MasterInit();
	//init_TIMER0_OVF(); 
	SEI();
	
    flag1=0;
    flag2=0;
    flag3=0;
    flagerr=0;
}
/***************ϵͳ��ʼ������ end ***********/
//����������===============================================
void key_scan(void)
{
    if((flag1 && (1 << keyprq_flag1)) == 0)  //���û�а�������
	{
	    if((PIND & (1<<key1)) == 0)  //�������԰���
		{
		    key_now = 1;  
		}
		else if((PIND & (1<<key2)) == 0)  //��ͣ����
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
//����������===============================================
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
			set_bit(flag1, keyeff_flag1);  //������Ч
		}
		if((flag1 & (1 << keyeff_flag1)) != 0)
		{
		    clr_bit(flag1, keyeff_flag1);
			switch(key_old)
			{
			    case 1:  //�������԰�������
				{
				    for(i = 0; i < 11; i++)
		            {
		                 SPI_putchar(ok[i]);
		            }
					master_statu = "receive";
				    beeperr3();				
				}break;
				case 2:  //��ͣ��������
				{
				    beeperr3();
				}break;
				default: break;
			}
		}
	}
}
/***************������ start *******************/
void main(void)
{
    //init_cpu();    //��ʼ��CPU
	SPI_MasterInit();
	PORTB = 0xff;
	DDRD  = 0x4c; //PIND0/1/4/5/7Ϊ����
	PORTD = 0x70;  //�������������������
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
		    clr_bit(PORTB, 0);  //���ʹӻ�ss���ţ�����ͨѶ
			SPI_MasterTransmit(0xFE); //������ͨ������MOSI���͵��ӻ�MISO
			delay_nms(500);
			set_bit(PORTB, 0);  //���ߴӻ�ss���ţ����ͨѶ
			SPSR &= ~(1 << SPIF);
			delay_nms(500);
			temp = SPI_MasterReceive(); //���մӻ�����
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



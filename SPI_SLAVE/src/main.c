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
char *slave_statu = "receive";  //����״̬���ж����ͻ��ǽ���
const uchar ok[] = {0xf3, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0a};
#define LED_BLINK() (PORTD^=0x40)

 /*SPI���дӻ���ʼ��*/
void SPI_SlaveInit(void)
{
    //MISO���������Ϊ����
	DDRB |= (1 << DDB3);
	//ʹ�ܴӻ�ģʽ
	SPCR = (1 << SPE);
}
/*SPI�ӻ�����һ���ַ�*/
void SPI_SlaveTransmit(unsigned char c)
{
    //�������ݴ���
	SPDR = c;
	//�ȴ����ͽ���
	while(!(SPSR & (1 << SPIF)));
}
/*SPI�ӻ�����һ���ַ�*/
unsigned char SPI_SlaveReceive(void)
{
    SPDR = 0;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}
#define SLAVE_RX_SIZE 11  //�������ջ�������С
unsigned char slave_rxBuffer[SLAVE_RX_SIZE]; //�������ջ�����
unsigned char slave_rxCounter = 0;  //�ѽ����ַ��ĸ���
#define SLAVE_TX_SIZE 11
unsigned char slave_txBuffer[SLAVE_TX_SIZE];
unsigned char slave_txCounter = 0, slave_wrIndex = 0, slave_rdIndex = 0;
#pragma interrupt_handler SPI_TransEND:18
void SPI_TransEND(void)
{
    if(slave_statu == "receive")
	{
	    uchar status,data;
	    status = SPSR; //SPI״̬�Ĵ���
	    data = SPDR; //SPI���ݼĴ���
	    if((flag2 & (1 << Slave_RECVFlag2)) == 0)  //�ж��Ƿ��������
	    {
			slave_rxBuffer[slave_rxCounter] = data;
			slave_rxCounter++;
			//�Զ�����շ�ʽ
			switch(slave_rxCounter)
			{
			    case 1: { //У����ʼ�ֽ�
				    if(data != 0xF3)  slave_rxCounter = 0;
				}break;
				case 2: {
				    if(data != 0xF4)  slave_rxCounter = 0;
				}break;
				case 11: {//У������ַ�
				    slave_rxCounter =0;
					if(data == 0x0A)  set_bit(flag2, Slave_RECVFlag2);
				}break;
				default:break;
			}
		}
	}
	if(slave_statu == "transmit")
	{
	    if(slave_txCounter) //���в�Ϊ��
		{
		    --slave_txCounter; //������
			SPDR = slave_txBuffer[slave_rdIndex];
			if(++slave_rdIndex == SLAVE_TX_SIZE)  slave_rdIndex = 0;			
		}
	}
}
//SPI����һ���ַ�
void SPI_putchar(unsigned char c)
{
    while(slave_txCounter == SLAVE_TX_SIZE);//�ȴ��������뷢�ͻ�����
	CLI();//�ر�ȫ���ж�ʹ��
	if(slave_txCounter || ((SPSR & (1 << SPIF)) == 0)) //��������Ϊ��
	{
	    slave_txBuffer[slave_wrIndex] = c;
		if(++slave_wrIndex == SLAVE_TX_SIZE)  slave_wrIndex = 0;
		++slave_txCounter;
	}
	else
	{
	    SPDR = c;
	}
	SEI();//��ȫ���ж�ʹ��
	//slave_statu = "receive";
}

//��ʱ��0��ʼ��
void init_TIMER0_OVF(void) 
{
    TCCR0 = 0x06; //256��Ƶ
	TCNT0 = 256 - CRYSTAL/256/2*0.5;  //0.5s��ʱ
	TIMSK |= (1<<TOIE0);  //��ʱ��0�ж�ʹ��
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
	
	TCNT0 = 256 - CRYSTAL/256/2*0.005;  //0.5s��ʱ
}

/***************ϵͳ��ʼ������ start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //����INT0~INT1�������ⲿ�ж�
    clr_bit(SFIOR,PUD); //������������������Ч
    DDRA=0xff;//1�������0������
    PORTA=0x00; //����PINC��PINFͨ·�л�
	
    DDRB=0xF8;  //1�������0������
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
	
	SPI_SlaveInit();
	
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
				    
					
				}break;
				case 2:  //��ͣ��������
				{
				    
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



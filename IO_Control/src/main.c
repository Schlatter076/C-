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

#define TRUE 1
#define FALSE 0
//PIND6���巢�ͽ�
#define LED_BLINK() (PORTD^=0x40)
//PB0���巢��
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
/***********USART0�����жϷ����� start**********************/ 
//USART���ջ�����
#define RX_BUFFER_SIZE 11                  //���ջ�������С���ɸ�����Ҫ�޸ġ�
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
			switch (rx_counter)
            {
                case 1: {     // ������ʼ�ַ�
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
                case 11: {    // ��������ַ�
							rx_counter = 0;
							if (data == END_TEXT) set_bit(flag1,Usart0_RECVFlag1);// Usart0_RecvFlag=1����ʾ��ȷ���յ�һ�����ݰ�
						}break;   
				default:break;
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
//��ʱ�жϺ���(�Զ������) 
#pragma interrupt_handler timer1_count_isr:15  
void timer1_count_isr(void) 
{ 
	PAULSE();
	TCNT1=65536-CRYSTAL/8/2*0.0005;
}
//��ʱ��1��ʼ��
void init_TIMER1_OVF(void)
{
	TCCR1B=0x02;  //8��Ƶ
	TCNT1=65536-CRYSTAL/8/2*0.5;//0.5s��ʱ
	TIMSK|=(1<<TOIE1);  //��ʱ��ʹ��
	SREG=0x80;      //ȫ��ʹ���ж�
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
	LED_BLINK();
	if((flag1&(1<<Usart0_RECVFlag1))!=0)
	{
	   IO_control();
	}
	//��ֹ����У��λ������ռλ
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
//������λ��ָ�����ö˿����
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
		//������ƿ�ʼ
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
		}//������ƽ���
		delay_nms(100);
		uart_ok();  //�����ݷ�������λ��
	}
	else
	    return;
}
//���;�Ե����okָ�����λ��
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
//���;�Ե����failָ�����λ��
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
//����IO�����óɹ�ָ�����λ��
void uart_ok(void) 
{
    uchar i = 0;
	for(i = 0; i < 11; i++) 
    {
	    USART0_putchar(ok[i]);
	}
}
//���ͼ�ͣ����������ָ�����λ��
void stopKey_clicked(void)
{
    uchar i = 0;
	for(i = 0; i < 11; i++)
	{
	    USART0_putchar(stop_key[i]);
	}
}
//���Ϳ�ʼ����������ָ�����λ��
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
/***************ϵͳ��ʼ������ start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //����INT0~INT1�������ⲿ�ж�
    clr_bit(SFIOR,PUD); //������������������Ч
    DDRA=0xff;//1�������0������
    PORTA=0x00; //����PINC��PINFͨ·�л�
	
    DDRB=0xff;  //1�������0������
    PORTB =0x00;
	
    DDRC=0xff; 
    PORTC =0x00; 

    DDRD  = 0x4c; //PIND0/1/4/5/7Ϊ����
	//PORTD = 0x30;
	PORTD = 0xf3;  //�������������������
	DDRE =0xfe;  //RXD0���룬��������Ч
	PORTE =0x01;  //TXD0���
	
	DDRF=0xff;  //PINFȫ�����
	PORTF =0x00;     
	
	DDRG =0xff;  
	PORTG =0x00;  //PING��ȫ���
	
	//CLI();
	init_usart0();
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
				    //startKey_clicked();
					for(count = 0; count < sizeof(secs); count++)
					{
					    seconds = secs[count];
						//delay_nms(200);
					}
				}break;
				case 2:  //��ͣ��������
				{
				    stopKey_clicked();
				}break;
				default: break;
			}
		}
	}
}
/***************������ start *******************/
void main(void)
{
    init_cpu();    //��ʼ��CPU
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



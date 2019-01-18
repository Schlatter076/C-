//���������������
#include <iom64v.h>
#include <stdio.h>
#include <macros.h>
#include <port.h>
#include <default.h>
#include <delay.h>
#include <EEPROM.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
//PIND6���巢�ͽ�
#define LED_BLINK() (PORTB^=0x10)
//PB0���巢��
#define PAULSE() (PORTB^=0x01)


//===========================================
uchar key_now = 0;
uchar key_old = 0;
uchar key_code = 0;
uchar key_cnt = 0;

#define KEY_COUNTER 10

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
//�Ƚ�ƥ��1��ʼ��
void init_OCR1A(void)
{
   TCCR1A=0x40;  //�Ƚ�ƥ��ʱOC1A��ƽȡ��
	TCCR1B=0x0A;  //CTC(�Ƚ�ƥ��ʱ���������)��8��Ƶ
	TCNT1=0;  //���������ֵ
	OCR1A=CRYSTAL/2.0/8/10000;  //�ȽϼĴ���OCR1A��ֵ(��ʱ���Ƶ��Ϊ6400 HZ)
}
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
	TCNT0 = 256 - CRYSTAL/256/2*0.001;  //0.5s��ʱ
}
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
    PORTB =0xf0;
	
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
				    
				}break;
				case 2:  
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
    init_cpu();    //��ʼ��CPU
		init_OCR1A();
	init_TIMER0_OVF();
	//init_TIMER1_OVF();
	//beeperr3();
	while(1)
    {
		key_scan();
		key_process();
		delay_nms(10);
	}
}



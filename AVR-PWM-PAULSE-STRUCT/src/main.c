/******************************************/
/*           ����������������޹�˾     */
/*Project:     ��λ�ɱ�PWM���η���        */
/*Guest:                                  */
/*Name:             main.c                */
/*Mcu chip:         Atmega64              */
/*Main clock:       �ⲿ����11.0592MHz    */
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
/***************�������� start**************/
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

//===============================���ڲ�������Start===================================
//����0���ջ�������С
#define RX_BUFFER_SIZE 11
//����0���ͻ�������С
#define TX_BUFFER_SIZE 11
//����1���ջ�������С
#define RX_BUFFER_SIZE_1 11
//����1���ͻ�������С
#define TX_BUFFER_SIZE_1 11
//����0�����ṹ��
struct
{
		unsigned char rx_buffer[RX_BUFFER_SIZE]; //���ջ�����
		unsigned char rx_counter; //���ջ������Ѷ�����
		unsigned char tx_buffer[TX_BUFFER_SIZE]; //���ͻ�����
		unsigned char tx_wr_index;  //���ͻ�����д�����
		unsigned char tx_rd_index; //���ͻ�������������
		unsigned char tx_counter; //���ͻ��������м���
}UART0;
//����1�����ṹ��
struct
{
		unsigned char rx_buffer[RX_BUFFER_SIZE_1]; //���ջ�����
		unsigned char rx_counter; //���ջ������Ѷ�����
		unsigned char tx_buffer[TX_BUFFER_SIZE_1]; //���ͻ�����
		unsigned char tx_wr_index;  //���ͻ�����д�����
		unsigned char tx_rd_index; //���ͻ�������������
		unsigned char tx_counter; //���ͻ��������м���
}UART1;
//===============================���ڲ�������End=====================================
/***********USART0�����жϷ����� start**********************/
//����һ����־λUsart0_RECVFlag1:=1��ʾ����0���յ���һ�����������ݰ�
//��port.h�ж���
#pragma interrupt_handler usart0_rxc_isr:19  //�����жϷ������
void usart0_rxc_isr(void)
{
    uchar status, data;
    status = UCSR0A;
    data = UDR0;
    if((flag1 & (1 << Usart0_RECVFlag1)) == 0) //�ж��Ƿ��������һ���µ����ݰ�
    {
        if ((status & (USART0_FRAMING_ERROR | USART0_PARITY_ERROR | USART0_DATA_OVERRUN)) == 0)
        {
            UART0.rx_buffer[UART0.rx_counter] = data;
            UART0.rx_counter++;
            switch (UART0.rx_counter)
            {
            case 1:       // ������ʼ�ַ�
            {
                if (data != 0xf3) UART0.rx_counter = 0;

            }
            break;
            case 2:
            {
                if (data != 0xf4) UART0.rx_counter = 0;

            }
            break;
            case 11:      // ��������ַ�
            {
                UART0.rx_counter = 0;
                if(data == 0x0a)
                    set_bit(flag1, Usart0_RECVFlag1); // Usart0_RecvFlag=1����ʾ��ȷ���յ�һ�����ݰ�
            }
            break;
            default:
                break;
            }
        }
    }
}
/***************USART0�����жϷ����� end**********************/
/*============================================================*/
/*============================================================*/
/***************USART0�����жϷ����� start********************/
#pragma interrupt_handler usart0_txc_isr:21  //�����жϷ������
void usart0_txc_isr(void)
{
    if (UART0.tx_counter)//���в�Ϊ��
    {
        --UART0.tx_counter;//������
        UDR0 = UART0.tx_buffer[UART0.tx_rd_index];
        if (++UART0.tx_rd_index == TX_BUFFER_SIZE) UART0.tx_rd_index = 0;
    }
}
/***********USART0�����жϷ����� end**********************/

/*============================================================*/
/***********USART0����һ���ַ����� start**********************/
void USART0_putchar(unsigned char c)
{
    while (UART0.tx_counter == TX_BUFFER_SIZE);
    CLI();//#asm("cli")�ر�ȫ���ж�����
    if (UART0.tx_counter || ((UCSR0A & USART0_DATA_REGISTER_EMPTY) == 0)) //���ͻ�������Ϊ��
    {
        UART0.tx_buffer[UART0.tx_wr_index] = c; //���ݽ������
        if (++UART0.tx_wr_index == TX_BUFFER_SIZE) UART0.tx_wr_index = 0; //��������
        ++UART0.tx_counter;
    }
    else
        UDR0 = c;
    SEI(); //#asm("sei")��ȫ���ж�����
}
/***********USART0���ͷ����� end**********************/
/*============================================================*/
/**
 * ����0����һ��ָ��
 * @buff ָ�����飬����ΪTX_BUFFER_SIZE
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
 * ����0���Ͷ��ָ��
 * @buff ָ�����飬����ΪTX_BUFFER_SIZE_1
 * @times ���͵Ĵ���
 * @dealys ÿ�η��͵�ʱ����(��λms����)
 */
void UART0_send_multiple(unsigned char buff[], unsigned char times, unsigned int delays)
{
    uchar i = 0;
    do
    {
        if(i++ >= times)
        {
            i = 0;
            break; //���ͳ�ʱ������ѭ��
        }
        UART0_send(buff);
        delay_nms(delays);
    }
    while((flag1 & (1 << Usart0_RECVFlag1)) == 0);
}
/*============================================================*/
/***************USART0��ʼ������ start*************************/
void init_usart0(void)
{
    UCSR0B = 0x00;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //�첽��8λ���ݣ�����żУ�飬һ��ֹͣλ���ޱ���
    UBRR0L = BAUD_L;                        //�趨������
    UBRR0H = BAUD_H;
    UCSR0A = 0x00;
    UCSR0B = (1 << RXCIE0) | (1 << TXCIE0) | (1 << RXEN0) | (1 << TXEN0); //0XD8  ���ա�����ʹ�ܣ� ���ж�ʹ��
}
/***************USART0��ʼ������ end***************************/
/***************USART1��ʼ������ start*************************/
void init_usart1(void)
{
    UCSR1B = 0x00;
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);  //�첽��8λ���ݣ�����żУ�飬һ��ֹͣλ���ޱ���
    UBRR1L = BAUD_L;                        //�趨������
    UBRR1H = BAUD_H;
    UCSR1A = 0x00;
    UCSR1B = (1 << RXCIE1) | (1 << TXCIE1) | (1 << RXEN1) | (1 << TXEN1); //0XD8  ���ա�����ʹ�ܣ� ���ж�ʹ��
}
/***************USART1��ʼ������ end***************************/
/***************USART1�����жϷ����� start********************/
#pragma interrupt_handler usart1_txc_isr:33  //�����жϷ������
void usart1_txc_isr(void)
{
    if (UART1.tx_counter)//���в�Ϊ��
    {
        --UART1.tx_counter;//������
        UDR1 = UART1.tx_buffer[UART1.tx_rd_index];
        if (++UART1.tx_rd_index == TX_BUFFER_SIZE_1) UART1.tx_rd_index = 0;
    }
}
/***********USART1�����жϷ����� end**********************/
/***********USART1����һ���ַ����� start**********************/
void USART1_putchar(unsigned char c)
{
    while (UART1.tx_counter == TX_BUFFER_SIZE);
    CLI();//#asm("cli")�ر�ȫ���ж�����
    if (UART1.tx_counter || ((UCSR1A & USART1_DATA_REGISTER_EMPTY) == 0)) //���ͻ�������Ϊ��
    {
        UART1.tx_buffer[UART1.tx_wr_index] = c; //���ݽ������
        if (++UART1.tx_wr_index == TX_BUFFER_SIZE_1) UART1.tx_wr_index = 0; //��������
        ++UART1.tx_counter;
    }
    else
        UDR1 = c;
    SEI(); //#asm("sei")��ȫ���ж�����
}
/***********USART1���ͷ����� end**********************/
/***********USART1�����жϷ����� start**********************/
//����һ����־λUsart1_RECVFlag1:=1��ʾ����1���յ���һ�����������ݰ�
//��port.h�ж���

#pragma interrupt_handler usart1_rxc_isr:31  //�����жϷ������
void usart1_rxc_isr(void)
{
    uchar status, data;
    status = UCSR1A;
    data = UDR1;
    if((flag1 & (1 << Usart1_RECVFlag1)) == 0) //�ж��Ƿ��������һ���µ����ݰ�
    {
        if ((status & (USART1_FRAMING_ERROR | USART1_PARITY_ERROR | USART1_DATA_OVERRUN)) == 0)
        {
            UART1.rx_buffer[UART1.rx_counter] = data;
            UART1.rx_counter++;
            switch (UART1.rx_counter)
            {
            case 1:       // ������ʼ�ַ�
            {
                if (data != 0xf3) UART1.rx_counter = 0;

            }
            break;
            case 2:
            {
                if (data != 0xf4) UART1.rx_counter = 0;

            }
            break;
            case 11:      // ��������ַ�
            {
                UART1.rx_counter = 0;
                if(data == 0x0a)
                    set_bit(flag1, Usart1_RECVFlag1); // Usart1_RecvFlag=1����ʾ��ȷ���յ�һ�����ݰ�
            }
            break;
            default:
                break;
            }
        }
    }
}
/***************USART1�����жϷ����� end**********************/
/**
 * ����1����һ��ָ��
 * @buff ָ�����飬����ΪTX_BUFFER_SIZE_1
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
 * ����1���Ͷ��ָ��
 * @buff ָ�����飬����ΪTX_BUFFER_SIZE_1
 * @times ���͵Ĵ���
 * @dealys ÿ�η��͵�ʱ����(��λms����)
 */
void UART1_send_multiple(unsigned char buff[], unsigned char times, unsigned int delays)
{
    uchar i = 0;
    do
    {
        if(i++ >= times)
        {
            i = 0;
            break; //���ͳ�ʱ������ѭ��
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
    TCCR1A = 0x54; //ͨ��A/B/C�Ƚ����ģʽ��8��Ƶ
    TCCR1B = 0x0a; //CTC�Ƚ�ƥ��ʱ��ƽȡ��
		TIMSK |= (1 << OCIE1A); //����Ƚ�Aƥ���ж�ʹ��
		SEI(); //ȫ���ж�ʹ��
		OCR1A = CRYSTAL / 8 / 2 / 50; //�Ƚ��������ֵ(20HZ)
}//*/
/**
 * ��ʼ��TC1���PWM
 */
void initTC1_PWM(void)
{
    TCCR1A = 0xa8; //����ƥ�����㣬����ƥ����λ(ICR1ģʽ)
    TCCR1B = 0x12; //OC1A OC1B OC1C�Ƚ������Ƶ����PWM 8��Ƶ
    TIMSK |= (1 << OCIE1A); //����Ƚ�Aƥ���ж�ʹ��
    ICR1 = ICR1 = CRYSTAL / 8 / 2 / 20; //��ʼ20HZ
    OCR1A = OCR1B = OCR1C = ICR1 / 2; //����ռ�ձ�Ϊ50%
    SEI(); //ȫ���ж�ʹ��
}
/**
 * ����TC1�������Ƶ��
 * @hz ����
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
 * ����TC1���PWM��
 * @TCCR1A_bit ���ͨ·ѡ��(7->ͨ·A 5->ͨ·B 3->ͨ·C)
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
 * ��ֹTC1���PWM��
 * @TCCR1A_bit ���ͨ·ѡ��(7->ͨ·A 5->ͨ·B 3->ͨ·C)
 */
void prohibitedTC1_PWM(unsigned char TCCR1A_bit)
{
    unsigned char sreg;
    sreg = SREG;
    CLI();
    TCCR1A &= ~(1 << TCCR1A_bit);
    SREG = sreg;
}
//TC1���ƥ��A�жϷ�����
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
/***************ϵͳ��ʼ������ start ***********/
void init_cpu(void)
{
    EIMSK = 0x00; //����INT0~INT1�������ⲿ�ж�
    clr_bit(SFIOR, PUD); //������������������Ч
    DDRA = 0xff; //1�������0������
    PORTA = 0x00; //LCD�����ݿ�

    DDRB = 0xff; //1�������0������
    PORTB = 0xff; //PB5/6/7��������ڣ�������

    DDRC = 0xff;
    PORTC = 0x00;

    DDRD  = 0x49;   //RXD1���룬��������Ч
    PORTD = 0xf4;   //TXD1�����

    DDRE = 0x0e;   //RXD0���룬��������Ч
    PORTE = 0x01;  //TXD0���

    DDRF = 0xff;
    PORTF = 0x00;

    DDRG = 0xf0;
    PORTG = 0x00; //PG3Ϊ��Ʒ��Ӧ������//PG4�̵���

    init_usart0();
    init_usart1();
    initTC1_PWM();
    SEI();

    flag1 = 0;
    flag2 = 0;
    flag3 = 0;
    flagerr = 0;
}
/***************ϵͳ��ʼ������ end ***********/
/*************������⺯�� start***************/
void key_scan(void)
{
    if((flag1 & (1 << keyprq_flag1)) == 0)
    {
        if((PIND & (1 << key1)) == 0) key_now = 1;   //auto����
        else if((PIND & (1 << key2)) == 0) key_now = 2; //men����
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
                if(key_cnt >= key_counter) //ƽ��״������10��,���ǲ���VR1ʱ����1��
                {
                    key_cnt = 0;
                    set_bit(flag1, keyprq_flag1);
                }
            }
        }
    }
}
/**************������⺯�� end****************/
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
/**************���������� start**************/
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
            case 1:  //auto����
            {
                paulse_change();
            }
            break;
            case 2:  //men����(����ѡ��)
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
/**************���������� end****************/

/***************������ start *******************/
void main(void)
{
    init_cpu();    //��ʼ��CPU
    while(1)
    {
        key_scan();
        key_process();
        delay_nms(10);
    }
}
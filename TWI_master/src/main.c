//====TWI���߲���
//====Loyer

#include <iom64v.h>
#include <port.h>
#include <delay.h>
#include <string.h>
#include <twi.h>

#define true 1
#define True 1
#define flase 0
#define False 0

//��������ַ
#define SL_ADDR 0x03   //0B00000011;����ӻ���ַ,ʹ�ܹ㲥����
//ͨ�ò���
#define Wait()  while((TWCR & (1 << TWINT))==0)
#define START() {TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);Wait();}
#define STOP()  (TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN))
#define WriteByte(x) {TWDR = (x); TWCR = (1 << TWINT) | (1 << TWEN); Wait();}
#define ReadByte() {}
#define ACK() (TWCR |= (1 << TWEA))
#define NACK() (TWCR &= ~(1 << TWEA))

void init_I2C(void) 
{
    DDRD = 0x00;
		PORTD = 0xFF;
}
int Master_Write(byte addr, byte data)
{
    START();  //����IIC
		if(TW_STATUS != TW_START) return 0;
		WriteByte(addr);
		if(TW_STATUS != TW_MT_SLA_ACK) return 0;
		WriteByte(data);
		if(TW_STATUS != TW_MT_DATA_ACK) return 0;
		STOP();
		delay_nms(2);
		return 1;
}
//INT2��ʼ��
void INT2_init(void)
{
    EICRA = 0x20;  //�ж�2Ϊ�½��ش���
		EIMSK = 0x04;  //�ж�2ʹ��
		SREG = 0x80;  //ȫ���ж�ʹ��
}
#pragma interrupt_handler INT2_vect:4
void INT2_vect(void)
{
    Master_Write(SL_ADDR, 0xEF);
}
void init_CPU(void)
{
    init_I2C();
		INT2_init();
}
void main(void)
{
		init_CPU();
		while(1)
		{
		    delay_nms(10);
		}
}
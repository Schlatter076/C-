/*TWI�������ͽ���*/
//��MAX6953���ӻ�Ϊ��
//Loyer

#include <twi.h>
#include <delay.h>
#include <iom64v.h>


//��������ַ
#define MAX6953R 0B10100001 //1=READ
#define MAX6953W 0B10100000 //0=WRITE
//ͨ�ò���
#define Wait()  while((TWCR & (1 << TWINT)) == 0)
#define START() {TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);Wait();}
#define STOP()  (TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN))
#define WriteByte(x) {TWDR = (x); TWCR = (1 << TWINT) | (1 << TWEN); Wait();}
#define ReadByte() {}
#define ACK() (TWCR |= (1 << TWEA))
#define NACK() (TWCR &= ~(1 << TWEA))

/*������д����*/
byte MAX6953_Write(byte addr, byte data)
{
    START();  //����
		if(TW_STATUS != TW_START) return 0;
		WriteByte(MAX6953W);  //����������ַ
		if(TW_STATUS != TW_MT_SLA_ACK)  return 0;
		WriteByte(addr);  //���ʹ������Ĵ�����ַ
		if(TW_STATUS != TW_MT_DATA_ACK)  return 0;
		WriteByte(data);  //��������
		if(TW_STATUS != TW_MT_DATA_ACK)  return 0;
		STOP();
		delay_nms(2);
		return 1;
}
//MAX6953��ʼ��
void MAX6953_init(void)
{
    MAX6953_Write(0x01, 0xFF);  //��λ0 1����������(�������)
		MAX6953_Write(0x02, 0xFF);  //��λ2 3����������(�������)
		MAX6953_Write(0x03, 0x03);  //����ɨ��λ����Χ(һ��4Ƭ��)
		MAX6953_Write(0x04, 0x01);  //���÷ǹض�ģʽ
		MAX6953_Write(0x07, 0x00);  //�����в���
}
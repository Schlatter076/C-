//====TWI���߲���
//====Loyer

#include <iom64v.h>
#include <port.h>
#include <delay.h>
#include <string.h>
#include <twi.h>
#include <default.h>

#define true 1
#define True 1
#define flase 0
#define False 0

byte count = 0;

#define SL_ADDR 0x03   //0B00000011;����ӻ���ַ,ʹ�ܹ㲥����
#define SR_INIT (TWCR = 0xC5);
//����ӻ���ַ��ʹ��TWI�ӻ�ģʽ
void Slave_init(void)
{
		TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //�����������е�ַ��TWIʹ�ܣ�TWI�ж�ʹ��
		TWAR = SL_ADDR;
		TWBR = 32;
}
#define BUFFER_SIZE 11  //��������С
byte buffer[BUFFER_SIZE];  //������
byte counter = 0;  //�����������ݸ���
#pragma interrupt_handler Slave_vect:34
void Slave_vect(void)
{
    switch (TW_STATUS)
    	{
    case TW_SR_SLA_ACK: //������յ�����SLA+W������ѷ�����ACK
			SR_INIT;  //TWINT TWEA TWEN TWIE��λ
			counter = 0;
    	break;
    case TW_SR_GCALL_ACK: //���յ��㲥��ַ�����ѷ���ACK
			SR_INIT;  //TWINT TWEA TWEN TWIE��λ
			counter = 0;
    	break;
		case TW_SR_DATA_ACK: //���յ�����������һ�����ݣ����ѷ���ACK
		    buffer[counter] = TWDR; //��ȡ����
			counter++;
			SR_INIT;
			break;
		case TW_SR_STOP:  //���յ�STOP���ظ�START
			SR_INIT;
			break;
		////////////////////////////////////////////////////
		case TW_ST_SLA_ACK: //�Լ���SLA+R�Ѿ�������ACK�ѷ���
			counter = 0;
			TWDR = 0xF3; //�ӻ�����
			//TWDR = buffer[counter];
			counter++;
			SR_INIT;
			break;
		case TW_ST_DATA_ACK: //�����Ѿ����ͽ��յ�ACK
			counter++;
			TWDR = 0xF4;
			//TWDR = buffer[counter];
			SR_INIT;
			break;
		case TW_ST_DATA_NACK: ////�����Ѿ����ͽ��յ�NOT_ACK
			SR_INIT;
			break;
		case TW_SR_GCALL_DATA_ACK:  //��ǰ�Թ㲥��ʽ��Ѱַ,�����Ѿ�������ACK�ѷ���
			buffer[counter] = TWDR;
			SR_INIT;
			counter++;
			break;			
    default: 
			SR_INIT;
    	break;
    	}
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
	for(count = 0; count < BUFFER_SIZE; count++)
	{
	    if(buffer[count] == 0xEF)
		   {
		       clr_bit(PORTB, PB6); //����LED
		   }
	}
	TCNT0 = 256 - CRYSTAL/256/2*0.5;  //��װ0.5s��ʱ
}

void init_CPU(void)
{
    DDRB = 0xFF;//�����
		PORTB = 0xFF; //������������LED����
		DDRD = 0x00; //SCL��SDA����Ϊ����
		PORTD = 0xFF; //�ڲ���������ʡȥ�ⲿ��������
		Slave_init();
		init_TIMER0_OVF();
}
void main(void)
{
    init_CPU();
		while(1)
		{
		    delay_nms(10);
		}
}
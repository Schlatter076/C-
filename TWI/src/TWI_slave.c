/*TWI�ӻ����ͽ���*/
//����ͨѶ
//Loyer

#include <twi.h>
#include <delay.h>
#include <iom64v.h>

#define SL_ADDR 0x03   //0B00000011;����ӻ���ַ,ʹ�ܹ㲥����
#define SR_INIT (TWCR = 0xC5);
//����ӻ���ַ��ʹ��TWI�ӻ�ģʽ
void Slave_init(void)
{
		TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE); //�����������е�ַ��TWIʹ�ܣ�TWI�ж�ʹ��
		TWAR = SL_ADDR;
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
void init_CPU(void)
{
    DDRD = 0x00; //SCL��SDA����Ϊ����
		PORTD = 0xFF; //�ڲ���������ʡȥ�ⲿ��������
		Slave_init();
}
/*TWI主机发送接收*/
//以MAX6953做从机为例
//Loyer

#include <twi.h>
#include <delay.h>
#include <iom64v.h>


//子器件地址
#define MAX6953R 0B10100001 //1=READ
#define MAX6953W 0B10100000 //0=WRITE
//通用操作
#define Wait()  while((TWCR & (1 << TWINT)) == 0)
#define START() {TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);Wait();}
#define STOP()  (TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN))
#define WriteByte(x) {TWDR = (x); TWCR = (1 << TWINT) | (1 << TWEN); Wait();}
#define ReadByte() {}
#define ACK() (TWCR |= (1 << TWEA))
#define NACK() (TWCR &= ~(1 << TWEA))

/*向器件写操作*/
byte MAX6953_Write(byte addr, byte data)
{
    START();  //启动
		if(TW_STATUS != TW_START) return 0;
		WriteByte(MAX6953W);  //发送器件地址
		if(TW_STATUS != TW_MT_SLA_ACK)  return 0;
		WriteByte(addr);  //发送从器件寄存器地址
		if(TW_STATUS != TW_MT_DATA_ACK)  return 0;
		WriteByte(data);  //发送数据
		if(TW_STATUS != TW_MT_DATA_ACK)  return 0;
		STOP();
		delay_nms(2);
		return 1;
}
//MAX6953初始化
void MAX6953_init(void)
{
    MAX6953_Write(0x01, 0xFF);  //数位0 1的亮度设置(最大亮度)
		MAX6953_Write(0x02, 0xFF);  //数位2 3的亮度设置(最大亮度)
		MAX6953_Write(0x03, 0x03);  //设置扫描位数范围(一共4片屏)
		MAX6953_Write(0x04, 0x01);  //设置非关断模式
		MAX6953_Write(0x07, 0x00);  //不进行测试
}
/*TWI.h文件定义*/
//Loyer
#ifndef _TWI_H
#define _TWI_H

/*开始*/
#define TW_START        0x08
/*重新开始*/
#define TW_REP_START        0x10
/*SLA + W ACK接收*/
 #define TW_MT_SLA_ACK       0x18
/*SLA+W transmitted, NACK received */
#define TW_MT_SLA_NACK      0x20
/*data transmitted, ACK received*/
#define TW_MT_DATA_ACK      0x28
/*data transmitted, NACK received*/
#define TW_MT_DATA_NACK     0x30
/*arbitration lost in SLA+W or data*/
#define TW_MT_ARB_LOST      0x38
/*arbitration lost in SLA+R or NACK*/
#define TW_MR_ARB_LOST      0x38
/*SLA+R transmitted, ACK received*/
#define TW_MR_SLA_ACK       0x40
/*SLA+R transmitted, NACK received*/
#define TW_MR_SLA_NACK      0x48
/*data received, ACK returned*/
#define TW_MR_DATA_ACK      0x50
/*data received, NACK returned*/
#define TW_MR_DATA_NACK     0x58
//SLA+R received, ACK returned
#define TW_ST_SLA_ACK       0xA8
//arbitration lost in SLA+RW, SLA+R received, ACK returned
#define TW_ST_ARB_LOST_SLA_ACK  0xB0
//data transmitted, ACK received
#define TW_ST_DATA_ACK      0xB8
//data transmitted, NACK received
#define TW_ST_DATA_NACK     0xC0
//last data byte transmitted, ACK received
#define TW_ST_LAST_DATA     0xC8
//SLA+W received, ACK returned
#define TW_SR_SLA_ACK       0x60
//arbitration lost in SLA+RW, SLA+W received, ACK returned
#define TW_SR_ARB_LOST_SLA_ACK  0x68
//general call received, ACK returned
#define TW_SR_GCALL_ACK     0x70
//arbitration lost in SLA+RW, general call received, ACK returned
#define TW_SR_ARB_LOST_GCALL_ACK 0x78
//data received, ACK returned
#define TW_SR_DATA_ACK      0x80
//data received, NACK returned
#define TW_SR_DATA_NACK     0x88
//general call data received, ACK returned
#define TW_SR_GCALL_DATA_ACK    0x90
//general call data received, NACK returned
#define TW_SR_GCALL_DATA_NACK   0x98
//stop or repeated start condition received while selected
#define TW_SR_STOP      0xA0
//no state information available
#define TW_NO_INFO      0xF8
//illegal start or stop condition
#define TW_BUS_ERROR        0x00
/**
* ingroup twi_h
* def TW_STATUS_MASK
* The lower 3 bits of TWSR are reserved on the ATmega163.
* The 2 LSB carry the prescaler bits on the newer ATmegas.
*/
#define TW_STATUS_MASK    ((1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3))
/**
* ingroup twi_h
* def TW_STATUS
*
* TWSR, masked by TW_STATUS_MASK
*/
#define TW_STATUS       (TWSR & TW_STATUS_MASK)
//SLA+R address
#define TW_READ     1
//SLA+W address
#define TW_WRITE    0

typedef unsigned char byte;

byte MAX6953_Write(byte addr, byte data);
void MAX6953_init(void);
void Slave_init(void);
void init_CPU(void);

#endif

















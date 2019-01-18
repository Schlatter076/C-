///////////////////////////////////////
#ifndef _DEFAULT_H
#define _DEFAULT_H

typedef unsigned char byte;
typedef unsigned int uint;

#define CRYSTAL 11059200    //ϵͳʱ��11.0592M

#define set_bit(x,y) (x|=(1<<y)) //��1����
#define clr_bit(x,y) (x&=~(1<<y)) //��0����
#define xor_bit(x,y) (x^=(1<<y)) //ȡ������
#define bit(x) (1<<x)            //��ĳλ����
#define get_bit(x,y) (x&=(1<<y)) //ȡ��ĳλ 

#endif

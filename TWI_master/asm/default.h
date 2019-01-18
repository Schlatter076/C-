///////////////////////////////////////
#ifndef _DEFAULT_H
#define _DEFAULT_H

typedef unsigned char byte;
typedef unsigned int uint;

#define CRYSTAL 11059200    //系统时钟11.0592M

#define set_bit(x,y) (x|=(1<<y)) //置1功能
#define clr_bit(x,y) (x&=~(1<<y)) //清0功能
#define xor_bit(x,y) (x^=(1<<y)) //取反功能
#define bit(x) (1<<x)            //对某位操作
#define get_bit(x,y) (x&=(1<<y)) //取出某位 

#endif

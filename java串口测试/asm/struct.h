/******************************************/
/*           广州旭东阪田电子有限公司     */
/*Project:     科牧自动测距FCT程序        */
/*Guest:                                  */
/*Name:             struct.c              */
/*Mcu chip:         Atmega64              */
/*Main clock:       外部晶体11.0592MHz    */
/*Rtc clock:                              */
/*Author:                                 */
/*Create date:      2018.05.24            */
/*Design date:                            */
/*Complete date:                          */
/******************************************/

#ifndef _STRUCT_H
#define _STRUCT_H
#define uchar unsigned char
typedef struct motorPozition Pozition;

struct motorPozition
{
    unsigned int M_X;
	unsigned int M_Y;
	unsigned int M_Z;
	uchar buf[20];
};
struct motorPozition getPozition(char *datas);

#endif

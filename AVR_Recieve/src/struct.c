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

#include <stdio.h>
#include <string.h>

struct motorPozition
{
    unsigned int M_X;
	unsigned int M_Y;
	unsigned int M_Z;
	unsigned char buf[20];
};

struct motorPozition getPozition(char *datas) 
{
    struct motorPozition tem;
	memcpy(tem.buf, datas, 20);  //操作非字符型数组
	//strcpy(tem.buf, datas);  //操作字符串
	return tem;
}
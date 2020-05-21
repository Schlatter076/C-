#include <stdio.h>
#include <unistd.h> //包含sleep函数
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define RETYR_TIMES 4
#define uchar unsigned char
#define uint unsigned int
#define INT32 unsigned long

unsigned char SET_ADCTest[] = {0XF3, 0XF4, 0X07, 0X47, 0X21, 0XFF, 0XF7, 0XFF, 0XFF, 0X63, 0XFC};
unsigned char command[] = {0XF3, 0XF4, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X0a};
uchar debugCommand[] = {0xF3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a};
uchar rx_buffer[] = {0xf3, 0xf4, 0x1f, 0x03, 0xff, 0xfc, 0x2f, 0x03, 0xff, 0xfc, 0x3f, 0x03, 0xff, 0xfc, 0x50, 0x0a};
INT32 val = 58764184;
const unsigned char SV0[] = "*RST";
const unsigned char SV1[] = "cmdset rigol";
const unsigned char SV2[] = ":FUNCtion:CLEAR";
const unsigned char SV3[] = ":function:voltage:DC";
const unsigned char SV4[] = ":measure:voltage:DC?";
const unsigned char SV6[] = ":calculate:function none";

void function(void (*fuc)())
{
    unsigned char cnt = 0;
    do
    {
        if(cnt++ >= RETYR_TIMES)
        {
            cnt = 0;
            break; //如果重试次数大于4,结束循环
        }
        (*fuc)();    //发送测试指令
        printf("\n");
        sleep(1);
    }
    while(1);
}
int sum(int a, int b)
{
    return a + b;
}
int add(int a1, int b1, int (*func)())
{
    return (*func)(a1, b1);
}
void SendSET_ADCTestcmd(void)
{
    unsigned char cmd_cnt = 0;
    for(cmd_cnt = 0; cmd_cnt < 11; cmd_cnt++)
    {
        printf("%4d\t", SET_ADCTest[cmd_cnt]);
    }
}

void fill_paulse(INT32 val)
{
    *(command + 2) = (val >> 24) & 0xff;
    *(command + 3) = (val >> 16) & 0xff;
    *(command + 4) = (val >> 8) & 0xff;
    *(command + 5) = val & 0xff;
    *(command + 9) = 0x03;
}
void fill_position(INT32 Y_val, INT32 Z_val)
{
    *(debugCommand + 1) = (Y_val >> 24) & 0xff; //取24-31位值
    *(debugCommand + 2) = (Y_val >> 16) & 0xff; //取16-23位值
    *(debugCommand + 3) = (Y_val >> 8) & 0xff; //取8-15位值
    *(debugCommand + 4) = Y_val & 0xff; //取0-7位值
    *(debugCommand + 5) = (Z_val >> 24) & 0xff; //取24-31位值
    *(debugCommand + 6) = (Z_val >> 16) & 0xff; //取16-23位值
    *(debugCommand + 7) = (Z_val >> 8) & 0xff; //取8-15位值
    *(debugCommand + 8) = Z_val & 0xff; //取0-7位值
    *(debugCommand + 9) = 0x30;
}
void fill_currentPosi()
{
    INT32 x, y, z;
    x = (rx_buffer[2] << 24) | (rx_buffer[3] << 16) | (rx_buffer[4] << 8) | rx_buffer[5];
    y = (rx_buffer[6] << 24) | (rx_buffer[7] << 16) | (rx_buffer[8] << 8) | rx_buffer[9];
    z = (rx_buffer[10] << 24) | (rx_buffer[11] << 16) | (rx_buffer[12] << 8) | rx_buffer[13];
    printf("x=%x,y=%x,z=%x", x, y, z);
}
typedef struct motorPosition
{
    INT32 Y;
    INT32 Z;
} Position;

/**
 * 发送字符串函数
 * @arrs 字符串
 */
void trans(const unsigned char* arrs)
{
    while(*(arrs++) != '\0')
    {
        printf("%c", *(arrs - 1));
    }
}
/**
 * 发送多字节函数
 * @arrs 字符数组
 */
void send(unsigned char* arrs)
{
    while(*(arrs++) != '\0')
    {
        printf("%02x ", *(arrs - 1));
    }
}

float bytestofloat(unsigned char *pch)//四字节内码转换为十进制浮点数
{
    float result;
    unsigned long result1;
    unsigned char *p;
    p = (unsigned char *)&result1;
    *p = *(pch + 3);
    *(p + 1) = *(pch + 2);
    *(p + 2) = *(pch + 1);
    *(p + 3) = *(pch);
    result = result1;
    return result;
}

union U
{
    float v;
    unsigned char buf[4];
}uu;

void printArr(unsigned char arr[], unsigned char size)
{
    unsigned char i = 0;
    for(i = 0; i < size; i++)
    {
        printf("%x\t", arr[i]);
    }
}

int main()
{
    /* double a = 123.45;
    double b = -1234.56;
    char ch = 5;
    char ptr[8];
    gcvt(a, 5, ptr);
    printf("a value=%s\n", ptr);
    gcvt(b, 6, ptr);
    printf("b value=%s\n", ptr);
    char *str = "WET";
    if(str == "WET")
    {
        printf("%s\n", str);
    }
    if(strcmp(str, "WET") == 0)
    {
        printf("相同:%s\n", str);
    } */
    float value;
    union U u2;
    int i;
    unsigned char buf[4] = {0x41, 0x82, 0x3B, 0x65};
    uu.buf[0] = 0x65;
    uu.buf[1] = 0x3B;
    uu.buf[2] = 0x82;
    uu.buf[3] = 0x41;
    
    float f_n = bytestofloat(buf);
    printf("f_n=%f\n", f_n);
    printf("%f\n", uu.v);
    if(uu.v > 16 && uu.v < 17)
    {
        printf("success!\n");
    }
    value = atof(buf);
    printf("atof(buf)=%f\n", value);
    
    u2.v = 16.279001;
    for(i = 0; i < 4; i++)
    {
        printf("%x\t", u2.buf[i]);
    }
    
    printf("\n==================================================\n");
    printArr(buf, sizeof(buf));
      
    
    
    getch(); //此函数包含在conio头文件中，用来程序等待
}
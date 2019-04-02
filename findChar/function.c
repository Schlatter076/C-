#include <stdio.h>
#include <unistd.h> //包含sleep函数
#include <conio.h>

#define RETYR_TIMES 4
#define uchar unsigned char
#define uint unsigned int
#define INT32 unsigned long

const unsigned char SET_ADCTest[11] = {0XF3, 0XF4, 0X07, 0X47, 0X21, 0XFF, 0XF7, 0XFF, 0XFF, 0X63, 0XFC};
uchar command[] = {0xf3, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a};
uchar debugCommand[] = {0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a};
uchar rx_buffer[] = {0xf3, 0xf4, 0x1f, 0x03, 0xff, 0xfc, 0x2f, 0x03, 0xff, 0xfc, 0x3f, 0x03, 0xff, 0xfc, 0x50, 0x0a};
INT32 val = 58764184;

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

int main()
{

    unsigned char cmd_cnt = 0 , test = 0xee;
    int tes = 0;
    //function(SendSET_ADCTestcmd);
    fill_paulse(val);
    for(cmd_cnt = 0; cmd_cnt < 11; cmd_cnt++)
    {
        printf("%4d\t", command[cmd_cnt]);
    }
    Position last_Pos = {500, 650};
    Position *Pos = &last_Pos;
    printf("\n%4d\n", Pos->Y);
    fill_position(85742, 20136);
    for(cmd_cnt = 0; cmd_cnt < 11; cmd_cnt++)
    {
        printf("%4d\t", debugCommand[cmd_cnt]);
    }//*/
    printf("\ntest>>3=%d\n", (test >> 3));
    tes = add(10, 20, sum);
    printf("function test:%d\n", tes);
    fill_currentPosi();
    printf("\nenter every Key to quit");
    getch(); //此函数包含在conio头文件中，用来程序等待
}
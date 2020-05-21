#include <stdio.h>
#include <conio.h>
#include <stdarg.h>

int SqSum(int num1, ...)
{
    va_list arg_ptr;
    int nSqlSum = 0, n = num1;
    va_start(arg_ptr, num1);
    while(*arg_ptr)
    {
        nSqlSum += (n * n);
        n = va_arg(arg_ptr, int);
    }
    va_end(arg_ptr);
    return nSqlSum;
}

void port_ctrl(int num, ...)
{
    va_list ptr;  //声明参数指针
    unsigned char addr, statu; //声明两个变量分别取地址和状态
    unsigned char port = 0;
    va_start(ptr, num); //设置参数指针

    while(num)
    {
        addr = va_arg(ptr, int); //取地址
        printf("addr=0x%02X\n", addr);
        statu = va_arg(ptr, int); //取状态
        printf("statu=0x%02X\n", statu);

        //(*(volatile unsigned char *)addr) = statu;
        port = statu;
        printf("0x%02X\n", port);

        num--;
    }

    va_end(ptr);

    //delay_nms(10); //延时等待端口设置稳定
    //send(ok);  //将数据反馈给上位机
}

int port_read(int num, ...)
{
    va_list ap;  //声明参数指针
    unsigned char addr; //声明变量取地址
    unsigned char index = 2;
    va_start(ap, num); //设置参数指针
    while(num)
    {
        addr = va_arg(ap, int); //取地址
        printf("addr=0x%02X\n", addr);
        index++;
        //input[index++] = (*(volatile unsigned char *)addr);
        num--;
    }
    va_end(ap);//释放指针资源

    //delay_nms(10);
    //send(input);
    return index - 2;
}

int main()
{
    int sum = SqSum(1, 3, 5, 7, 9);
    printf("1+3+5+7+9=%d\n", sum);
    //printf("0x%02x\n", 0x66);
    port_ctrl(2, 0x38, 0x66, 0x39, 0x77, 0x00, 0x00); //, 0x39, 0x77, 0x3a, 0x88, 0x40, 0x99
    printf("===================================================\n");
    sum = port_read(3, 0x38, 0x66, 0x39, 0x40, 0x44);
    printf("实际操作个数:%d\n", sum);
    getch();
}
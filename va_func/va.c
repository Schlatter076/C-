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
    va_list ptr;  //��������ָ��
    unsigned char addr, statu; //�������������ֱ�ȡ��ַ��״̬
    unsigned char port = 0;
    va_start(ptr, num); //���ò���ָ��

    while(num)
    {
        addr = va_arg(ptr, int); //ȡ��ַ
        printf("addr=0x%02X\n", addr);
        statu = va_arg(ptr, int); //ȡ״̬
        printf("statu=0x%02X\n", statu);

        //(*(volatile unsigned char *)addr) = statu;
        port = statu;
        printf("0x%02X\n", port);

        num--;
    }

    va_end(ptr);

    //delay_nms(10); //��ʱ�ȴ��˿������ȶ�
    //send(ok);  //�����ݷ�������λ��
}

int port_read(int num, ...)
{
    va_list ap;  //��������ָ��
    unsigned char addr; //��������ȡ��ַ
    unsigned char index = 2;
    va_start(ap, num); //���ò���ָ��
    while(num)
    {
        addr = va_arg(ap, int); //ȡ��ַ
        printf("addr=0x%02X\n", addr);
        index++;
        //input[index++] = (*(volatile unsigned char *)addr);
        num--;
    }
    va_end(ap);//�ͷ�ָ����Դ

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
    printf("ʵ�ʲ�������:%d\n", sum);
    getch();
}
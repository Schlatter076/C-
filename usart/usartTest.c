#include <stdio.h>
#include <string.h>

#define CMD_SIZE 15
const unsigned char start[CMD_SIZE] = {0x60, 0x00, 0x81, 0x00, 0x68, 0x00, 0x84, 0x00, 0x79, 0x00, 0x80, 0x00, 0x67, 0x00, 0x62};
const unsigned char stop[CMD_SIZE] = {0x60, 0x00, 0x83, 0x00, 0x50, 0x00, 0x67, 0x00, 0x83, 0x00, 0x87, 0x00, 0x67, 0x00, 0x62};
const unsigned char R_measure[] = {0x60, 0x00, 0x67, 0x00, 0x83, 0x00, 0x68, 0x00, 0x90, 0x00, 0x83, 0x00, 0x49, 0x00, 0x62};
char *test(char *tem);
struct ret test_1(char *tem);
struct ret
{
  char buf[30];
};
int main(void)
{
  printf("%4d", sizeof(R_measure));
  int i = sizeof(R_measure) + CMD_SIZE;
  printf("\n%4d", i);
  char* motor_statu = "Running";
  printf("\n%d", sizeof(motor_statu));
  if(i > 15) motor_statu = "Stop";
  printf("\n%d", sizeof(motor_statu));
  printf("\n%s", motor_statu);
  printf("\n%d", *motor_statu);//非法操作
  printf("%s", test("\njust test\n"));
  struct ret b;
  b = test_1("struct test");
  printf("%s\n", b.buf);
  int a = 20;
  int* bp = &a;
  printf("%4d\n", bp);
  printf("%4d\n", *bp);
  return 0;
}
//以指针作为返回值
char *test(char *tem)
{
  return tem;
}
struct ret test_1(char *tem)
{
  struct ret a;
  strcpy(a.buf, tem);
  return a;
}
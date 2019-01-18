#include <stdio.h>
#include <string.h>

#define qianjin 1
#define STATUS_OK 0x01
#define STATUS_NG 0x00


typedef struct motor_position 
{
  unsigned int X;
  unsigned int Y;
  unsigned int Z;  
}Point;

Point last_position = {0, 0, 0};
Point now_position = {0, 0, 0};

int main(void)
{
  printf("%d,%d,%d\n", last_position.X, last_position.Y, last_position.Z);
  printf("%d,%d,%d\n", now_position.X, now_position.Y, now_position.Z);
  if(qianjin == 1)
    printf("%s\n", "qianjin");
  last_position.Y = 0x35;
  now_position.X = 0x55;
  Point *p = &last_position;
  Point *p1 = &now_position;
  p->X = 1;
  p1->Y = 10;
  printf("%d,%d,%d\n", last_position.X, last_position.Y, last_position.Z);
  printf("%d,%d,%d\n", now_position.X, now_position.Y, now_position.Z);
  
}
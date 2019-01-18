#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

///////////////////////////////////////////////////////////////////
/*
 * 用来查找value是否存在于source中,不存在返回FALSE
 * 如果存在，则返回TRUE
 * count是字符串的个数
 */
int find_char(char **strings, char value, int count) 
{
  char *tem;
  int num;
  while((tem = *strings++) != NULL)
  {
    num++;
    while(*tem != '\0')
    {
      if(*tem++ == value)
        return TRUE;
    }
    if(num >= count)
      break;
  }
  return FALSE;
}
///////////////////////////////////////////////////////////////////
/*
 * c库函数实现字符查找的函数
 */
char *str_by_cLib(char *string, int value)
{
  while(*string && *string != (char)value)
    *string++;
  if(*string == value)
    return string;
  else
    return NULL;
}
///////////////////////////////////////////////////////////////////
/*
 * 用来查找chars是否存在于source中,不存在返回NULL
 * 如果存在，则返回其第一次出现的位置
 */
char *find_chars(char *source, char *chars)
{
  if(source == NULL || chars == NULL)
  {
    return NULL;
  }
  char *str;
  while(*source && *source != *chars)
    *source++;
  if(*source == *chars)
  {
    str = source; //记录第一次发现相同字符的指针
    while(*str && *str == *chars)
    {
      *str++;
      *chars++;
    }
    if(*str == '\0' || *chars == '\0') //字符串到达末尾
    {
      if(*--str == *--chars)
        return source; //如果子字符串完全被包含，则返回找到第一个字符相同的位置
      else
        return NULL;
    }
    else
      return NULL;
  }
  else
    return NULL;
}
///////////////////////////////////////////////////////////////////
/*
 * 字符串连接函数
 */
char *str_cat(char *s1, char *s2)
{
  char *p = s1;
  while(*s1 != '\0')
    *s1++;
  while(*s2 != '\0')
  {
    *s1++ = *s2++;
  }
  *s1 = '\0';
  return p;
}
///////////////////////////////////////////////////////////////////
/*
 * 如果字符串中包含字符c，则删除第一次出现的字符c
 */
void del_char(char *str, char c)
{  
  char *ret = str;
  char *dst = str;
  while(*ret)
  {
    if(*ret != c)
      *dst++ = *ret;
    
    if(*ret == c) //只删除第一次出现的字符
    {
      break;
    }
    ret++;
  }
  *dst = '\0';
  str = str_cat(str, ++ret);
}
///////////////////////////////////////////////////////////////////
/*
 * 如果字符串中包含字符c，则删除字符串中所有的字符c
 */
void del_chars(char *str, char c)
{  
  char *ret = str;
  char *dst = str;
  while(*ret)
  {
    if(*ret != c)
      *dst++ = *ret;

    ret++;
  }
  *dst = '\0';
}
///////////////////////////////////////////////////////////////////
/*
 * 如果字符串中包含子字符串substr，则删除该字符串(连续的字符串)
 */
void del_subStr(char *str, char *substr)
{
  char *tem = str;
  char *p = str;
  while(*p != '\0')
    *p++;
  *p--;
  while(*tem && *tem != *substr)
    *tem++;
  while(*substr != '\0' && *substr != *p)
  {
    del_char(tem, *substr);
    substr++;
    p--;
  }
}
///////////////////////////////////////////////////////////////////
/*
 * 如果字符串中包含子字符串substr，则删除该字符串中所有substr中的字符
 */
void del_subChars(char *str, char *substr)
{
  char *tem = str;
  char *p = str;
  while(*p != '\0')
    *p++;
  *p--;
  while(*tem && *tem != *substr)
    *tem++;
  while(*substr != '\0' && *substr != *p)
  {
    del_chars(tem, *substr);
    substr++;
    p--;
  }
}
///////////////////////////////////////////////////////////////////
/*
 * 在字符串中插入一个字符
 */
void insert_char(char *str, char c, int index)
{
  char *p = str;
  char *p1 = str;
  while(index--)
  {
    *p++;
  }
  p1 = p;
  *p++ = '\0';
  *++p1 = c;
  str_cat(str, p1);
}
/////////////////////////////////////////////////////////////////////
/*
 * 字符串反转函数(不知道为什么，字符串字符数量超过20会有问题)
 */
char *reverse_str(char *str)
{
  char *start = str;
  char *end;
  char *mid;
  while(*str != '\0')
  {
    *str++;
  }
  end = --str;  //end指向最后一个字符
  mid = end;
  while(*str != *start)
  {
    *end++ = *str--;
  }
  *end++ = *str--;  //循环会少一次，这里保证将第一个字符加到末尾
  return mid;
}
/////////////////////////////////////////////////////////////////////
int sum(int a, int b)
{
    return a + b;
}                                      //以函数作为参数，传递给函数
int test(int a, int b, int (*fuc)())
{
    return (*fuc)(a, b);
}
/////////////////////////////////////////////////////////////////////
























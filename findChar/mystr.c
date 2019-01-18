#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

///////////////////////////////////////////////////////////////////
/*
 * ��������value�Ƿ������source��,�����ڷ���FALSE
 * ������ڣ��򷵻�TRUE
 * count���ַ����ĸ���
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
 * c�⺯��ʵ���ַ����ҵĺ���
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
 * ��������chars�Ƿ������source��,�����ڷ���NULL
 * ������ڣ��򷵻����һ�γ��ֵ�λ��
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
    str = source; //��¼��һ�η�����ͬ�ַ���ָ��
    while(*str && *str == *chars)
    {
      *str++;
      *chars++;
    }
    if(*str == '\0' || *chars == '\0') //�ַ�������ĩβ
    {
      if(*--str == *--chars)
        return source; //������ַ�����ȫ���������򷵻��ҵ���һ���ַ���ͬ��λ��
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
 * �ַ������Ӻ���
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
 * ����ַ����а����ַ�c����ɾ����һ�γ��ֵ��ַ�c
 */
void del_char(char *str, char c)
{  
  char *ret = str;
  char *dst = str;
  while(*ret)
  {
    if(*ret != c)
      *dst++ = *ret;
    
    if(*ret == c) //ֻɾ����һ�γ��ֵ��ַ�
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
 * ����ַ����а����ַ�c����ɾ���ַ��������е��ַ�c
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
 * ����ַ����а������ַ���substr����ɾ�����ַ���(�������ַ���)
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
 * ����ַ����а������ַ���substr����ɾ�����ַ���������substr�е��ַ�
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
 * ���ַ����в���һ���ַ�
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
 * �ַ�����ת����(��֪��Ϊʲô���ַ����ַ���������20��������)
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
  end = --str;  //endָ�����һ���ַ�
  mid = end;
  while(*str != *start)
  {
    *end++ = *str--;
  }
  *end++ = *str--;  //ѭ������һ�Σ����ﱣ֤����һ���ַ��ӵ�ĩβ
  return mid;
}
/////////////////////////////////////////////////////////////////////
int sum(int a, int b)
{
    return a + b;
}                                      //�Ժ�����Ϊ���������ݸ�����
int test(int a, int b, int (*fuc)())
{
    return (*fuc)(a, b);
}
/////////////////////////////////////////////////////////////////////
























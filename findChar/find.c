#include <stdio.h>
//#define NDEBUG
//#include <assert.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

/*
 * 用来查找value是否存在于source中,不存在返回FALSE
 * 如果存在，则返回TRUE
 * count是字符串的个数
 */
int find_char_1(char **strings, char value, int count) 
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
void find_char_1_test(void)
{
  char *str_arr[] = {"abcdefgh", "jklmnopq", "rstuvwxyz", "234567891"};
  char **str = str_arr;
  char value = 'i';
  int temp = find_char_1(str, value, 4);
  if(temp == 0)
    printf("str_arr中不包含%c\n", value);
  else
    printf("str_arr中包含%c\n", value);
  char values = 'p';
  int temps = find_char_1(str, values, 4);
  if(temps == 0)
    printf("str_arr中不包含%c\n", values);
  else
    printf("str_arr中包含%c\n", values);
}
char *str_by_cLib(char *string, int value)
{
  while(*string && *string != (char)value)
    *string++;
  if(*string == value)
    return string;
  else
    return NULL;
}
void str_by_cLib_test(void)
{
  char *str = "qwertyuiop";
  int ch = 'u';
  char *tem = str_by_cLib(str, ch);
  if(tem != NULL)
    printf("在%s中找到%c,在%d处\n", str, ch, (tem - str + 1));
  else
    printf("没有找到指定的值%c\n", ch);
  int v = 'j';
  char *temp = str_by_cLib(str, v);
  if(temp != NULL)
    printf("在%s中找到%c,在%d处\n", str, v, (temp - str + 1));
  else
    printf("没有找到指定的值%c\n", v);
}
/*
 * 用来查找chars是否存在于source中,不存在返回NULL
 * 如果存在，则返回其第一次出现的位置
 */
char *find_char(char *source, char *chars)
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
void char_test(void)
{
  char *sou = "what is can i do for you ?";
  char *ch = "for you";
  char *s = "how can";
  char *hah = "you";
  char *str = find_char(sou, ch);
  if(str != NULL)
  {
    printf("found '%s' in %d at '%s'\n", ch, (str - sou + 1), sou); //打印第一次出现的位置
  }
  else
    printf("can not found %s in %s!\n", ch, sou);
  
  char *str_1 = find_char(sou, s);
  if(str_1 != NULL)
  {
    printf("found '%s' in %d at '%s'\n", s, (str_1 - sou + 1), sou); //打印第一次出现的位置
  }
  else
    printf("can not found %s in %s!\n", s, sou);
  
  char *str_2 = find_char(sou, hah);
  if(str_2 != NULL)
  {
    printf("found '%s' in %d at '%s'\n", hah, (str_2 - sou + 1), sou); //打印第一次出现的位置
  }
  else
    printf("can not found %s in %s!\n", hah, sou);
}
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
  //str = strcat(str, ++ret);
  str = str_cat(str, ++ret);
  //*/
  /*
  char *save;
  int n;
  char *ptr = str_by_cLib(str, (int)c);
  for( ; *str != '\0'; str++)
  {
    if(*str == c)
    {
      save = str + 1;
      while(*save == c)
        save++;
      n = save - str;
      save = str;
      for( ; *save != '\0'; save++)
        *save = *(save + n);
    }
  }
  //*/
}
void delChar(char arr[], char c) 
{
  int i, j;
  for(i = 0, j = 0; arr[i] != '\0'; i++)
  {
    if(arr[i] != c)
    {
      arr[j++] = arr[i]; 
    }
  }
  arr[j] = '\0';
}
void del_char_test(void)
{
  char arr[] = "asdfghjklmk";
  char c = 'k';
  del_char(arr, c);
  int i = 0;
  while(arr[i] != '\0')
  {
    printf("%c,", arr[i]);
    i++;
  }
}
/*
 * 如果str里包含substr，则删除substr第一次出现在str中的子字符串，返回剩余字符串
 * 否则返回待删除字符串
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
void del_subStr_test(void)
{
  char arr[] = "what is can i do for you ?";
  char *ch = "do";
  del_subStr(arr, ch);
  printf("%s\n", arr);
}//*/
int sum(int a, int b)
{
    return a + b;
}
int test(int a, int b, int (*fuc)())
{
    return (*fuc)(a, b);
}
void func_test(void)
{
  int (*func)() = sum;
  //func为指向函数sum的指针
  int integer = test(10, 20, func); //以函数作为函数参数
  printf("%d\n", integer);
}
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
  //str_cat(++end, start);
  //start = end;
  while(*str != *start)
  {
    *end++ = *str--;
  }
  *end++ = *str--;  //循环会少一次，这里保证将第一个字符加到末尾
  return mid;
}
int main(void) 
{
  /*
  printf("函数地址作参数:\n");
  func_test();
  printf("自定义查找一个字符:\n");
  find_char_1_test();
  printf("c语言库函数之字符查找:\n");
  str_by_cLib_test();
  printf("查找字符串:\n");
  char_test();
  
  printf("字符串删除测试\n");
  del_subStr_test();
  del_char_test();
  //*/
  //char str[] = "abcdefghijk";
  char str[] = "piuytrefdgfhbvcxghkj";
  //char *str = "abcdefghijk";
  printf("%s\n", str);
  char *p = reverse_str(str);
  //insert_char(str, '6', 6);
  printf("%s\n", p);
  int a, b = 5;
  a = b--;
  printf("%d\n", a);
  printf("%s\n", "xhfjfkdishurjnghnfgju");
}
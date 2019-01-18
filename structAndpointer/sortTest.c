#include <stdlib.h>
#include <stdio.h>


/*
 * 比较整数函数，大于返回1，小于返回-1，否则返回0
 */
int compare_integers(const void *a, const void *b)
{
  return (*(int *)a - *(int *)b); 
}

int main(void) 
{
  int *array;
  int n_values;
  int i;
  
  /* 观察有多少个值 */
  printf("How many values are there? ");
  if(scanf("%d", &n_values) != 1 || n_values <= 0)
  {
    printf("Illegal number of values\n");
    exit(EXIT_FAILURE);
  }
  
  /* 分配内存，用来存储这些值 */
  array = (int *)malloc(n_values * sizeof(int));
  if(array == NULL)
  {
    printf("Can't get memory for that many values. \n");
    exit(EXIT_FAILURE);
  }
  
  /* 读取这些数值 */
  for(i = 0; i < n_values; i++) 
  {
    printf("? ");
    if(scanf("%d", array + i) != 1)
    {
      printf("Error reading value #%d\n", i);
      free(array);
      exit(EXIT_FAILURE);
    }
  }
  /* 进行排序 */
  qsort(array, n_values, sizeof(int), compare_integers);
    
  /* 打印值 */
  for(i = 0; i < n_values; i++)
  {
    printf("%d\n", array[i]);
  }
    
  /* 释放内存并退出 */
  free(array);
  return EXIT_SUCCESS;
  
}
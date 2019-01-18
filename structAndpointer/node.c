#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/* 定义一个结构体，表示链表中的节点，存储的值为整数 */
typedef struct NODE
{
  struct NODE *link;
  int value;
} Node;
/* 定义一个链表插入函数，插入成功返回TRUE */
int insert_ver1(Node **rootp, int new_value)
{
  Node *pre;
  Node *current;
  Node *new;
  
  //得到指向第一个节点的指针
  current = *rootp;
  pre = NULL;
  
  /* 顺序访问链表，直到其值大于或者等于新插入值的节点 */
  while(current != NULL && current -> value < new_value)
  {
    pre = current;
    current = current -> link;
  }
  
  /* 为新节点分配内存，并把新值存到新节点中，如果内存分配失败，函数返回FALSE */
  new = (Node *)malloc(sizeof(Node));
  if(new == NULL)
    return FALSE;
  new -> value = new_value;
  
  /* 将新节点插入到链表中，并返回TRUE */
  new -> link = current;
  if(pre == NULL)
    *rootp = new;
  else
    pre -> link = new;
  return TRUE;  
}

/* 定义一个链表插入函数，插入成功返回TRUE */
int insert_ver2(register Node **linkp, int new_value)
{
  register Node *current;
  register Node *new;
  
  /* 顺序访问链表，直到其值大于或者等于新插入值的节点 */
  while((current = *linkp) != NULL && current -> value < new_value)
  {
    linkp = &current -> link; //取当前节点的指针
  }
  /* 为新节点分配内存，并把新值存到新节点中，如果内存分配失败，函数返回FALSE */
  new = (Node *)malloc(sizeof(Node));
  if(new == NULL)
    return FALSE;
  new -> value = new_value;
  
  /* 将新节点插入到链表中，并返回TRUE */
  new -> link = current;
  *linkp = new;
  return TRUE;  
}

/*
 * 创建一个链表
 */
Node *creat_node(void)
{
  Node *next = NULL; //下一个节点的指针
  Node *tail = NULL; //末尾的指针
  int i, len, data; //循环变量，链表长度，节点数据
  
  Node* head = (Node *)malloc(sizeof(Node));
  if(head == NULL)
  {
    printf("Create LinkedList FAILED!\n");
    exit(EXIT_FAILURE);
  }
  head -> value = 0;
  head -> link = NULL;
  tail = head;
  
  printf("Please enter the lengh of linkedList: ");
  scanf("%d", &len);
  for(i = 1; i < len + 1; i++)
  {
    next = (Node*)malloc(sizeof(Node));
    if(next == NULL)
    {
      printf("Can't get memory! \n");
      exit(EXIT_FAILURE);
    }
    printf("Please enter value of the node_%d: ", i);
    scanf("%d", &data);
    next -> value = data;
    next -> link = NULL;
    
    tail -> link = next;
    tail = next;
  }
  return head;
  
}

void print_node(Node *head)
{
  Node *node = head -> link;
  while(node != NULL)
  {
    printf("%4d", node -> value);
    node = node -> link;
  }
  putchar('\n');
}

int main(void)
{
  int insert;
  Node *head = creat_node();
  printf("Create LinkedList!\n");
  print_node(head);
  printf("Please enter the number insert into LinkedList: ");
  scanf("%d", &insert);
  insert_ver2(&head, insert);
  print_node(head);
}












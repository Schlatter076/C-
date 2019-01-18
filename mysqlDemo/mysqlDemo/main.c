#include <stdio.h>   
#include <stdlib.h>   
#include <mysql.h>  //包含MySQL所需要的头文件
#include <WinSock2.h> //Socket，连接MySQL也需要用的

      MYSQL  mysql,  *sock;
    //声明MySQL的句柄     int main(void)  {       const char * host = "127.0.0.1";  //因为是作为本机测试，所以填写的是本地IP
      const char * user = "root";
        //这里改为你的用户名，即连接MySQL的用户名
      const char * passwd = "yangfamily";
  //这里改为你的用户密码
      const char * db = "testmysql";
       //这里改为你要连接的数据库的名字
      unsigned int  port = 3306;
            //这是MySQL的服务器的端口，如果你没有修改过的话就是3306。
      const char * unix_socket = NULL;
     //unix_socket这是unix下的，我在Windows下，所以就把它设置为NULL
      unsigned long  client_flag = 0;
       //这个参数一般为0
        const char * i_query = "select * from test";
  //查询语句
        MYSQL_RES * result;
                           //保存结果集的
      MYSQL_ROW  row;
                                //代表的是结果集中的一行
        mysql_init(&mysql);                          //连接之前必须使用这个函数来初始化
      if ( (sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag) ) == NULL ) //连接MySQL
       {          printf("连接失败，原因是: \n");          fprintf(stderr, " %s\n", mysql_error(&mysql));          exit(1);      }       else       {          fprintf(stderr, "连接MySQL成功！！\n");      }        if ( mysql_query(&mysql, i_query) != 0 )        //如果连接成功，则开始查询
       {          fprintf(stderr, "查询失败！\n");          exit(1);      }       else       {          if ( (result = mysql_store_result(&mysql)) == NULL )  //保存查询的结果
	           {              fprintf(stderr, "保存结果集失败！\n");              exit(1);          }           else           {              while ( (row = mysql_fetch_row(result)) != NULL )  //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】
		               {                  printf("name is %s\t", row[0]);                //打印当前行的第一列的数据
			                  printf("age is %s\t\n", row[1]);              //打印当前行的第二列的数据
			              }          }        }       mysql_free_result( result);
                                 //释放结果集
       mysql_close( sock);
                                         //关闭连接
      system("pause");       exit( EXIT_SUCCESS);
  }
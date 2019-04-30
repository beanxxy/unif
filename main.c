#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <mysql/mysql.h>

#define PORT 9001
#define QUEUE_MAX_COUNT 5
#define BUFF_SIZE 1024
#define SERVER_STRING "Server: hoohackhttpd/0.1.0\r\n"


MYSQL conn;
MYSQL_RES *res;
MYSQL_ROW row;
void printfdb(char *p){
    printf("%s",p);
}
int key=0;
void testdb(){
	key=1;
	mysql_init(&conn);
	mysql_real_connect(&conn,
		"127.1",
		"root",
		"123",
    "wms",3306,NULL,0);
    printf("accept a client\n");
    mysql_query(&conn,"select * from user where id=1");
    res=mysql_store_result(&conn);
    if(res!=NULL){
        while(row=mysql_fetch_row(res)){
            printf("%s\n",row[0]);
        }
    }else{
        printf("error:%s",mysql_error(&conn));
    }
}
void testServer(){
/* 定义server和client的文件描述符 */
    int server_fd = -1;
    int client_fd = -1;

    u_short port = PORT;
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    char buf[BUFF_SIZE];
    char recv_buf[BUFF_SIZE];
    char hello_str[] = "Hello world!";

    int hello_len = 0;

    /* 创建一个socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(-1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    /* 设置端口，IP，和TCP/IP协议族 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* 绑定套接字到端口 */
    if (bind(server_fd, (struct sockaddr *)&server_addr,
         sizeof(server_addr)) < 0) {
        perror("bind");
        exit(-1);
    }

    /* 启动socket监听请求，开始等待客户端发来的请求 */
    if (listen(server_fd, QUEUE_MAX_COUNT) < 0) {
        perror("listen");
        exit(-1);
    }

    printf("http server running on port %d\n", port);
	if(key==0)testdb();
    while (1) {

        /* 调用了accept函数，阻塞了程序，直到接收到客户端的请求 */
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                   &client_addr_len);
        if (client_fd < 0) {
            perror("accept");
            exit(-1);
        }
        
        printf("client socket fd: %d\n", client_fd);
        /* 调用recv函数接收客户端发来的请求信息 */
        hello_len = recv(client_fd, recv_buf, BUFF_SIZE, 0);

        printf("receive %d\n", hello_len);

        printf("%s",recv_buf);
        /* 发送响应给客户端 */
        sprintf(buf, "HTTP/1.0 200 OK\r\n");
        send(client_fd, buf, strlen(buf), 0);
        strcpy(buf, SERVER_STRING);
        sprintf(buf,"Set-Cookie:xxx=xxx;xxy=fd;\r\n");
        send(client_fd, buf, strlen(buf), 0);
        sprintf(buf, "Content-Type: text/html;charset=utf-8\r\n");
        send(client_fd, buf, strlen(buf), 0);
        strcpy(buf, "\r\n");
        send(client_fd, buf, strlen(buf), 0);
        sprintf(buf, "你好Hello World\r\n");
        send(client_fd, buf, strlen(buf), 0);
        /* 关闭客户端套接字 */
        close(client_fd);
    }
    lose(server_fd);
}
void startServer(){
/* 定义server和client的文件描述符 */
    int server_fd = -1;
    int client_fd = -1;
    u_short port = PORT;
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    char buf[BUFF_SIZE];
    char recv_buf[BUFF_SIZE];
    int recv_len = 0;

    /* 创建一个socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(-1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    /* 设置端口，IP，和TCP/IP协议族 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* 绑定套接字到端口 */
    if (bind(server_fd, (struct sockaddr *)&server_addr,
         sizeof(server_addr)) < 0) {
        perror("bind");
        exit(-1);
    }

    /* 启动socket监听请求，开始等待客户端发来的请求 */
    if (listen(server_fd, QUEUE_MAX_COUNT) < 0) {
        perror("listen");
        exit(-1);
    }
    printf("http server running on port %d\n", port);
    while(1){
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        /* 调用recv函数接收客户端发来的请求信息 */
        send(client_fd, buf, strlen(buf), 0);
        recv_len = recv(client_fd, recv_buf, BUFF_SIZE, 0);
        close(client_fd);
    }
}
int main()
{
    return 0;
}

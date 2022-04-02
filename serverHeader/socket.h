#ifndef SOCKET_H
#define SOCKET_H

#include <Winsock2.h>
#include <iostream>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#define SERV_PORT 9000
#define IP "127.0.0.1"
#define MAX_CLIENT 30

//Summary: TCP连接windows初始化参数
void tcpWSA();
//Summary: TCP连接sockaddr等参数初始化函数，返回socket
SOCKET tcpInit();
//Summary: TCP连接监听socket端口函数，返回socket
SOCKET tcpAccept(SOCKET serverSocket);
//Summary: TCP连接申请连接函数
SOCKET tcpConnect();

#endif
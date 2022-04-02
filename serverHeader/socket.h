#ifndef SOCKET_H
#define SOCKET_H

#include <Winsock2.h>
#include <iostream>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#define SERV_PORT 9000
#define IP "127.0.0.1"
#define MAX_CLIENT 30

//Summary: TCP����windows��ʼ������
void tcpWSA();
//Summary: TCP����sockaddr�Ȳ�����ʼ������������socket
SOCKET tcpInit();
//Summary: TCP���Ӽ���socket�˿ں���������socket
SOCKET tcpAccept(SOCKET serverSocket);
//Summary: TCP�����������Ӻ���
SOCKET tcpConnect();

#endif
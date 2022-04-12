#ifndef SOCKET_H
#define SOCKET_H

#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <Windows.h>

#define SERV_PORT 9000
#define IP "127.0.0.1"

void tcpWSA();
SOCKET tcpInit();
SOCKET tcpAccept(SOCKET serverSocket);
SOCKET tcpConnect();

#endif
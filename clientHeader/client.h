#include <winsock.h> //socketͷ�ļ�
#pragma comment (lib,"ws2_32.lib") //����socket
#include <cstring>
#include <iostream>
#include <json/json.h>

#define BUFSIZE 1024

class Client
{
public:
    Client();
    ~Client();

    void sendData(std::string str);
    void recvData();
private:
    char recvBuf[BUFSIZE];
    SOCKET clientSocket;
};


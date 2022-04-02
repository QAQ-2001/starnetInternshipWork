#include "client.h"
#include "socket.h"

Client::Client()
{
	memset(recvBuf, 0, sizeof(char) * BUFSIZE);

	tcpWSA();

	clientSocket = tcpConnect();
}

Client::~Client()
{
	//¹Ø±ÕÌ×½Ó×Ö
	closesocket(clientSocket);

	WSACleanup();
}

void Client::sendData(std::string str) 
{
	std::cout << str << std::endl;
	send(clientSocket, str.c_str(), str.size(), 0);
}

void Client::recvData() 
{
	recv(clientSocket, recvBuf, BUFSIZE, 0);
	std::cout << recvBuf << std::endl;
}

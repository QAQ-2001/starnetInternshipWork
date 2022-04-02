#ifndef SERVER_H
#define SERVER_H

#define NOMINMAX

#include <cstring>

#include <event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

#include <json/json.h>

#include "sOperation.h"
#include "socket.h"
//#include "database.h"

#define BUFSIZE 1024

//tcp�����ʵ����
class Server
{
public:
    Server();
    ~Server();

private:

    struct sockaddr_in serverAddr;
    event_base* base;
    evconnlistener* listener;

    static AdoAccess dataBase;
    
    //Summary: TCP���ӽ�����Ļص�����
    //Parameters:
    //      listener: �¼�������
    //      fd: socket�˿�
    //      addr: sockaddr��
    //      socklen: ����addr�Ĵ�С
    //      arg: ����event_base��
    //Return: void
    static void listenerCb(evconnlistener* listener, evutil_socket_t fd, sockaddr* addr, int socklen, void* arg);

    //Summary: д��bufferevent��������ص�����
    //Parameters:
    //      bev: bufferevent����
    //      ctx: socket�˿�
    //Return: void
    static void readCb(bufferevent* bev, void* ctx);

    //Summary: �������˴�����ص�����
    //Parameters:
    //      bev: bufferevent����
    //      what: ������Ϣ
    //      ctx: socket�˿�
    //Return: void
    static void eventCb(bufferevent* bev, short what, void* ctx);

    //Summary: �ӵ��ͻ��������Ĵ�����
    //Parameters:
    //      recv: ��string�����json��ʽ��Ϣ
    //Return: void
    static void function(std::string recv);

};

#endif
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

//tcp服务端实现类
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
    
    //Summary: TCP连接建立后的回调函数
    //Parameters:
    //      listener: 事件监听器
    //      fd: socket端口
    //      addr: sockaddr类
    //      socklen: 参数addr的大小
    //      arg: 传递event_base类
    //Return: void
    static void listenerCb(evconnlistener* listener, evutil_socket_t fd, sockaddr* addr, int socklen, void* arg);

    //Summary: 写入bufferevent缓冲区后回调函数
    //Parameters:
    //      bev: bufferevent对象
    //      ctx: socket端口
    //Return: void
    static void readCb(bufferevent* bev, void* ctx);

    //Summary: 服务器端错误处理回调函数
    //Parameters:
    //      bev: bufferevent对象
    //      what: 错误信息
    //      ctx: socket端口
    //Return: void
    static void eventCb(bufferevent* bev, short what, void* ctx);

    //Summary: 接到客户端请求后的处理函数
    //Parameters:
    //      recv: 用string储存的json格式信息
    //Return: void
    static void function(std::string recv);

};

#endif
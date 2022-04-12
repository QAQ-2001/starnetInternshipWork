#ifndef SERVER_H
#define SERVER_H

#define NOMINMAX

#include <cstring>

#include <event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

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

    static AdoAccess database;
    
    //Summary: TCP连接建立后的回调函数
    //Parameters:
    //      listener: 事件监听器
    //      fd: socket端口
    //      addr: sockaddr类
    //      socklen: 参数addr的大小
    //      arg: 传递event_base类
    //Return:  void
    static void listenerCb(evconnlistener* listener, evutil_socket_t fd, sockaddr* addr, int socklen, void* arg);

    //Summary: 写入bufferevent缓冲区后回调函数
    //Parameters:
    //      bev: bufferevent对象
    //      ctx: socket端口
    //Return:  void
    static void readCb(bufferevent* bev, void* ctx);

    //Summary: 服务器端错误处理回调函数
    //Parameters:
    //      bev: bufferevent对象
    //      what: 错误信息
    //      ctx: socket端口
    //Return:  void
    static void eventCb(bufferevent* bev, short what, void* ctx);

    //Summary: 接到客户端请求后的处理函数
    //Parameters:
    //      recv: 用string储存的json格式信息
    //Return:  
    //      sOperation.h文件中各个函数的返回值----成功或错误详情见该头文件
    //      json error----执行错误,json格式数据读取失败
    //      no function----执行错误,收到的json数据中的function项问空
    //      function error----执行错误,sOperation.h头文件中的函数未正确执行
    static std::string function(std::string recv);
};

#endif
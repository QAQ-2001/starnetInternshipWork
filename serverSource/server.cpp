#include "server.h"

#pragma warning(disable : 4996)

AdoAccess Server::dataBase = AdoAccess::AdoAccess();

Server::Server()
{   
    tcpWSA();

    base = event_base_new();
    if (nullptr == base) {
        puts("Couldn't open event base");
        return;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERV_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(IP);

    //创建socket,绑定，监听和接收连接
    //创建监听对象，在指定的地址上监听接下来的TCP连接
    //该函数自动bind,listen,accept，回调函数做自己的事即可
    //标志含义为释放监听对象关闭socket端口重复使用
    listener = evconnlistener_new_bind(base, listenerCb, (void*)base,
        LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, MAX_CLIENT, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (nullptr == listener)
    {
        std::cout << "evconnlistener_new_bind error" << std::endl;
        return;
    }

    //监听集合中的事件
    event_base_dispatch(base);
}

Server::~Server()
{
    evconnlistener_free(listener);
    event_base_free(base);
}

void Server::listenerCb(evconnlistener* listener, evutil_socket_t fd, sockaddr* addr, int socklen, void* arg)
{
    std::cout << "接收" << fd << "的连接" << std::endl;

    event_base* base = (event_base*)arg;

    //对已存在的socket创建bufferevent对象
    //标志含义为如果释放bufferevent对象则关闭连接
    bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (bev == nullptr)
    {
        std::cout << "bufferevent_socket_new error" << std::endl;
        return;
    }

    //给bufferevent设置回调函数
    //写事件回调函数，读事件回调函数和其它事件回调函数
    bufferevent_setcb(bev, readCb, NULL, eventCb, (void*)fd);

    //使能bufferevent对象
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    return;
}

void Server::readCb(bufferevent* bev, void* ctx)
{
    char buf[1024] = { 0 };
    evutil_socket_t fd = (evutil_socket_t)ctx;
    size_t ret = bufferevent_read(bev, buf, sizeof(buf));
    if (ret < 0)
    {
        std::cout << "bufferevent_read error" << std::endl;
        return;
    }
    else
    {
        std::cout << "read" << fd << ": " << buf << std::endl;
        std::string tmpBuf = buf;
        function(tmpBuf);
        return;
    }
}

void Server::eventCb(bufferevent* bev, short what, void* ctx)
{
    evutil_socket_t fd = (evutil_socket_t)ctx;
    if (what & BEV_EVENT_EOF)
    {
        std::cout << "客户端" << fd << "下线" << std::endl;
        bufferevent_free(bev);
        return;
    }
    else
    {
        std::cout << "未知错误" << std::endl;
        bufferevent_free(bev);
        return;
    }
}

void Server::function(std::string recv)
{
    Json::Value root;
    Json::String errs;
    Json::CharReaderBuilder readBuilder;
    std::unique_ptr<Json::CharReader> jsonRead(readBuilder.newCharReader());
    if (nullptr == jsonRead) {
        std::cerr << "jsonRead is null" << std::endl;
        return;
    }

    // reader将Json字符串解析到root，root将包含Json里所有子元素
    bool ret = jsonRead->parse(recv.c_str(),recv.c_str() + recv.length(), &root, &errs);
    if (!ret || !errs.empty()) {
        std::cout << "parseJsonFromString error!" << errs << std::endl;
        return;
    }

    std::string func = root["function"].asString();
    if ("login" == func)
    {
        std::string result = serverLogin(root, dataBase);
        std::cout << result << std::endl;
    }


}


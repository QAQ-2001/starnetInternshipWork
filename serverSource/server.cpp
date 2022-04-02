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

    //����socket,�󶨣������ͽ�������
    //��������������ָ���ĵ�ַ�ϼ�����������TCP����
    //�ú����Զ�bind,listen,accept���ص��������Լ����¼���
    //��־����Ϊ�ͷż�������ر�socket�˿��ظ�ʹ��
    listener = evconnlistener_new_bind(base, listenerCb, (void*)base,
        LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, MAX_CLIENT, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (nullptr == listener)
    {
        std::cout << "evconnlistener_new_bind error" << std::endl;
        return;
    }

    //���������е��¼�
    event_base_dispatch(base);
}

Server::~Server()
{
    evconnlistener_free(listener);
    event_base_free(base);
}

void Server::listenerCb(evconnlistener* listener, evutil_socket_t fd, sockaddr* addr, int socklen, void* arg)
{
    std::cout << "����" << fd << "������" << std::endl;

    event_base* base = (event_base*)arg;

    //���Ѵ��ڵ�socket����bufferevent����
    //��־����Ϊ����ͷ�bufferevent������ر�����
    bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (bev == nullptr)
    {
        std::cout << "bufferevent_socket_new error" << std::endl;
        return;
    }

    //��bufferevent���ûص�����
    //д�¼��ص����������¼��ص������������¼��ص�����
    bufferevent_setcb(bev, readCb, NULL, eventCb, (void*)fd);

    //ʹ��bufferevent����
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
        std::cout << "�ͻ���" << fd << "����" << std::endl;
        bufferevent_free(bev);
        return;
    }
    else
    {
        std::cout << "δ֪����" << std::endl;
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

    // reader��Json�ַ���������root��root������Json��������Ԫ��
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


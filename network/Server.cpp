#include "Server.h"
#include "netsys.h"
#include "Socket.h"
#include "Session.h"
#include "Packet.h"
#include "SockAddr.h"
#include "Mutex.h"
#include "Thread.h"
#include "Logger.h"

namespace kit {

IServer::IServer()
: socket_(NULL)
, active_(false)
, del_socket_mutex_(NULL)
{
}

IServer::~IServer()
{
    if (active_)
    {
        shutdown();
    }

    KIT_SAFE_RELEASE(socket_)
    KIT_SAFE_RELEASE(del_socket_mutex_)
}

bool IServer::baseInit()
{
    Terminal::baseInit();

    del_socket_mutex_ = Mutex::create(false);

    memset(sockets_, 0, sizeof(Socket*) * SERVER_CONNECTION_LENGTH);

    return true;
}

int32_t IServer::startup(const char* ip, int32_t port)
{
	if (socket_)
	{
		shutdown();
	}

    // 创建sockaddr
    SockAddr* addr = SockAddr::create();
    addr->init(ip, port, KIT_AF_INET);
    if (!addr->valid())
    {
        ERR("[IServer](startup) sockaddr error! %s, %d", ip, port);
        return -1;
    }

    // 创建socket
    Socket* sock = Socket::create();
    sock->init(KIT_AF_INET, KIT_SOCK_STREAM, 0);

    if (!sock->valid())
    {
        ERR("[IServer](startup) socket error! %s, %d", ip, port);
        return -2;
    }

    // socket bind
    if (sock->bind(addr) < 0)
    {
        ERR("[IServer](startup) bind error! %s, %d", ip, port);
        return -3;
    }

    // socket listen
    if (sock->listen(SERVER_LISTEN_LENGTH) < 0)
    {
        ERR("[IServer](startup) listen error! %s, %d", ip, port);
        return -4;
    }

    sock->retain();
    socket_ = sock;

    addSocket(sock->getHandle(), sock);

    active_ = true;

    return 0;
}

void IServer::handlePollEvent()
{
    // 处理epoll事件
    PollEvent ev;
    int32_t cnt = event_que_.count();
    SocketID sock_fd;
    int32_t sock_ev;
    Socket* sock;

    for (int i = 0; i != cnt; ++i)
    {
        if (!event_que_.pop(ev))
            break;
        sock_fd = (SocketID)ev.fd;
        sock_ev = ev.events;
        sock = getSocket(sock_fd);
        if (sock == NULL || sock == socket_)
        {
            continue;
        }
        // 客户端消息
        if (sock_ev & KIT_POLLIN)
        {
            // 收到协议包处理
            Session* session = sock->getSession();
            if (session)
            {
                sock->pullPacketsToSession();
                handleSessionRecv(session);
            } else {
                handleSocketRecv(sock);
            }
        }
        if ((sock_ev & KIT_POLLOUT) && !sock->ready_out)
        {
            sock->ready_out = true;
            int32_t ret = sock->flushSend();
            if (ret == SOCKET_SEND_ERROR)
            {
                sock_ev |= KIT_POLLERR;
            } else {
            }
        }
        if (sock_ev & KIT_POLLERR)
        {
            // 真正删除
            delSocket(sock_fd);
            // 告诉session掉线
        }

        DBG("[Server](update) poll event fd=%d", ev.fd);
    }
}

// 处理session前的协议
void IServer::handleSocketRecv(Socket* sock)
{
    PacketQue& packet_que = sock->getRecvQueue();
    Packet* packet;
    int count = packet_que.count();
    for (int i = 0; i != count; ++i)
    {
        if (!packet_que.pop(packet))
            break;
        if (packet->getPID() == kit::C_CONNECT)
            pk_C_CONNECT(sock, packet);
        packet->release();
    }
}

void IServer::update()
{
    Terminal::update();
    handlePollEvent();
}

//@thread
int32_t IServer::addSocket(SocketID fd, Socket* sock)
{
    Socket* old_sock = getSocket(fd);
    if (old_sock)
    {
        // del old
        delSocket(fd);
    }
    if (sock == NULL)
    {
        // create socket
        sock = Socket::create();
        sock->init(fd);
    }

    sock->deleted = false;
    sock->retain();
    sockets_[fd] = sock;
    return 0;
}

//@thread
int32_t IServer::delSocket(SocketID fd)
{
    AutoLock lock(del_socket_mutex_);

    Socket* sock = getSocket(fd);
    if (sock)
    {
        sock->close();
        sock->deleted = true;
        sock->release();
        sockets_[fd] = NULL;
        return 0;
    }
    else
    {
        return -1;
    }
}

Socket* IServer::getSocket(SocketID fd)
{
    return sockets_[fd];
}

int32_t IServer::shutdown()
{
    active_ = false;
	if (socket_)
	{
		socket_->close();
        socket_->release();
		socket_ = NULL;
	}

    Socket* sock;
    for (int i = 0; i != SERVER_CONNECTION_LENGTH; ++i)
    {
        sock = sockets_[i];
        if (sock)
        {
            sock->close();
            sock->release();
        }
    }

    return 0;
}

void IServer::pk_C_CONNECT(Socket* socket, Packet* packet)
{
    //TODO: valid packet
    Session* session = Session::create(false);
    session->setSocket(socket);
    addSession(session);
    session->release();
    handleSessionRecv(session);

    //TODO: packet pool
    Packet* pack = Packet::create(false);
    pack->init(kit::S_CONNECT, nullptr);
    sessionSendPacket(session, pack);
    pack->release();

    DBG("__________pk_C_CONNECT___________");
}

}


#include "Client.h"
#include "ClientState.h"
#include "netsys.h"
#include "Socket.h"
#include "SockAddr.h"
#include "Packet.h"
#include "Session.h"
#include "Logger.h"

namespace kit {

IClient::IClient()
: state_mgr_(NULL)
, socket_(NULL)
, session_(NULL)
, active_(false)
{
}

IClient::~IClient()
{
    KIT_SAFE_RELEASE(socket_);
    KIT_SAFE_RELEASE(session_);
}

bool IClient::baseInit()
{
    if (! Terminal::baseInit())
        return false;
    state_mgr_ = new ClientStateMgr(this);
    state_mgr_->setState(ClientState::Inited);
    return true;
}

int32_t IClient::startup(const char* ip, int32_t port)
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
        ERR("[IClient](startup) sockaddr error! %s, %d", ip, port);
        return -1;
    }

    // 创建socket
    Socket* sock = Socket::create();
    sock->init(KIT_AF_INET, KIT_SOCK_STREAM, 0);

    if (!sock->valid())
    {
        ERR("[IClient](startup) socket error! %s, %d", ip, port);
        return -2;
    }

    sock->setAddr(addr);

    // connect交付到线程去做
    /*
    if (sock->connect(addr) < 0)
    {
        ERR("[IClient](startup) connect error! %s, %d", ip, port);
        return -3;
    }
    */

    sock->retain();
    socket_ = sock;

    active_ = true;
    state_mgr_->setState(ClientState::Connecting);
    return 0;
}

void IClient::handlePollEvent()
{
    int32_t cnt = event_que_.count();
    if (cnt == 0)
        return;
    Socket* sock = socket_;
    // 处理epoll事件
    PollEvent ev;
    int32_t sock_ev;

    for (int i = 0; i != cnt; ++i)
    {
        if (!event_que_.pop(ev))
            break;
        sock_ev = ev.events;
        if (sock_ev & KIT_POLLIN)
        {
            Session* session = socket_->getSession();
            if (session)
            {
                socket_->pullPacketsToSession();
                handleSessionRecv(session);
            } else {
                handleSocketRecv(socket_);
            }
        }
        if (sock_ev & KIT_POLLOUT)
        {
            sock->ready_out = true;
            int32_t ret = sock->flushSend();
            if (ret == -1)
            {
                sock_ev |= KIT_POLLERR;
            } else {
            }
        }
        if (sock_ev & KIT_POLLERR)
        {
            // 真正删除
            socket_->release();
            socket_ = NULL;
            // 告诉session掉线
        }

        DBG("[Server](update) poll event fd=%d", ev.fd);
    }
}

// 处理session前的协议
void IClient::handleSocketRecv(Socket* sock)
{
    PacketQue& packet_que = sock->getRecvQueue();
    Packet* packet;
    int count = packet_que.count();
    for (int i = 0; i != count; ++i)
    {
        if (!packet_que.pop(packet))
            break;
    }
}

void IClient::update()
{
    Terminal::update();
    handlePollEvent();
}

int32_t IClient::shutdown()
{
    active_ = false;
	if (socket_)
	{
		socket_->close();
        socket_->release();
		socket_ = NULL;
	}

    return 0;
}

void IClient::sendProtocol(const Protocol* protocol)
{
    if (!socket_)
    {
        ERR("[IClient](sendProtocol) not inited socket!");
        return;
    }
    Session* session = socket_->getSession();
    if (!session)
    {
        ERR("[IClient](sendProtocol) not inited session!");
        return;
    }
    sessionSendProtocol(session, protocol);
}

}


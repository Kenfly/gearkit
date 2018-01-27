#include "Terminal.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "Socket.h"
#include "Session.h"
#include "Protocol.h"
#include "Logger.h"

namespace kit {

Terminal::Terminal()
: packet_handler_(NULL)
{
}

Terminal::~Terminal()
{
    KIT_SAFE_RELEASE(packet_handler_);
    clearProtocols();
    clearSessions();
}

bool Terminal::baseInit()
{
    return true;
}

void Terminal::handleRecvPackets(Socket* socket)
{
    PacketHandler* handler = packet_handler_;
    if (handler == NULL)
    {
        ERR("[Terminal](handleRecvPackets) no packet handler!");
        return;
    }

    PacketQue& que = socket->getRecvQueue();
    Packet* pack = NULL;
    int cnt = que.count();
    for (int i = 0; i != cnt; ++i)
    {
        if (!que.pop(pack))
            break;
        // TODO: sid
        handler->onPacket(0, pack);
        pack->release();
    }
}

void Terminal::setPacketHandler(PacketHandler* handler)
{
    KIT_SAFE_RELEASE(packet_handler_)
    packet_handler_ = handler;
    KIT_SAFE_RETAIN(packet_handler_)
}

void Terminal::addProtocol(ProtocolID pid, Protocol* pto)
{
    delProtocol(pid);
    protocol_map_.set(pid, pto);
    KIT_SAFE_RETAIN(pto);
}

void Terminal::delProtocol(ProtocolID pid)
{
    protocol_map_.del(pid, [](Protocol* p){
        KIT_SAFE_RELEASE(p);
    });
}

Protocol* Terminal::getProtocol(ProtocolID pid) const
{
    Protocol* pto = NULL;
    protocol_map_.get(pid, pto);
    return pto;
}

Session* Terminal::getSession(SessionID sid) const
{
    Session* sd = NULL;
    session_map_.get(sid, sd);
    return sd;
}

SessionID Terminal::addSession(Socket* sock, SessionID sid)
{
    Session* sd = NULL;
    if (sid != SIDNULL)
    {
        if (!session_map_.get(sid, sd))
        {
            ERR("[Terminal](addSession)faild! no session:%d", sid);
            return DSESSIONERR;
        }
        sd->setSocket(sock);
    }
    else
    {
        sd = Session::create();
        sd->init(sock);
    }
    delSession(sid);
    session_map_.set(sid, sd);
    KIT_SAFE_RETAIN(sd);
    return sd->getID();
}

void Terminal::delSession(SessionID sid)
{
    session_map_.del(sid, [](Session* p){
        KIT_SAFE_RELEASE(p);
    });
}

void Terminal::sendProtocol(SessionID sid, const Protocol* pto)
{
    Session* sd = getSession(sid);
    if (sd == NULL)
    {
        ERR("[Terminal](sendProtocol) no session:%d\n", sid);
        return;
    }
    Packet* pack = Packet::create();
}

void Terminal::recvProtocol(SessionID sid, const Protocol* pto)
{
}

void Terminal::clearProtocols()
{
    protocol_map_.clear([](Protocol* p){
        KIT_SAFE_RELEASE(p);
    });
}

void Terminal::clearSessions()
{
    session_map_.clear([](Session* p){
        KIT_SAFE_RELEASE(p);
    });
}

} // namespace kit


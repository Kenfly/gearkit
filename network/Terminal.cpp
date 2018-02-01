#include "Terminal.h"
#include "Packet.h"
#include "Socket.h"
#include "Session.h"
#include "Protocol.h"
#include "BufferPool.h"
#include "Logger.h"

namespace kit {

Terminal::Terminal()
{
}

Terminal::~Terminal()
{
    clearProtocols();
    clearSessions();
}

bool Terminal::baseInit()
{
    return true;
}

void Terminal::handleSession(Session* session)
{
    PacketQue& que = session->getPackets();
    Packet* pack = NULL;
    int cnt = que.count();
    for (int i = 0; i != cnt; ++i)
    {
        if (!que.pop(pack))
            break;
        ProtocolID pid = pack->getPID();
        Protocol* pto;
        if (!protocol_map_.get(pid, pto))
        {
            pack->release();
            continue;
        }
        pto->unserialize(pack->getBuffer());
        pack->release();
        recvProtocol(session->getID(), pto);
    }
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

void Terminal::addSession(Session* session)
{
    SessionID sid = session->getID();
    delSession(sid);
    session_map_.set(sid, session);
    KIT_SAFE_RETAIN(session);
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
    Buffer* buf = g_BufPool->createBuffer(pto->getBudgetSize());
    pto->serialize(buf);
    pack->init(pto->getPID(), buf);
    sd->sendPacket(pack);
}

void Terminal::recvProtocol(SessionID sid, const Protocol* pto)
{
    DBG(pto->toString().c_str());
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


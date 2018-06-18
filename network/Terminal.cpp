#include "Terminal.h"
#include "Packet.h"
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

void Terminal::update()
{
    Ref::update();
    handleOutSession();
}

void Terminal::handleSessionRecv(Session* session)
{
    PacketQue& que = session->getRecvPackets();
    Packet* pack = NULL;
    int cnt = que.count();
    for (int i = 0; i != cnt; ++i)
    {
        if (!que.pop(pack))
            break;
        ProtocolID pid = pack->getPID();
        Protocol* protocol;
        if (!protocol_map_.get(pid, protocol))
        {
            pack->release();
            //TODO: not exist protocol
            continue;
        }
        protocol->unserialize(pack->getBuffer());
        pack->release();
        recvProtocol(session->getID(), protocol);
    }
}

void Terminal::handleSessionSend(Session* session)
{
	if (! session->flush())
		session->out_dirty = false;
}

void Terminal::handleOutSession()
{
	size_t length = out_vec_.size();
    if (length == 0)
        return;
	for (size_t i = 0; i != length;)
	{
		Session* session = out_vec_[i];
		handleSessionSend(session);
		if (!session->out_dirty)
		{
			out_vec_[i] = out_vec_[length];
			out_vec_[length - 1] = session;
			--length;
		}
		else
			++i;
	}
    out_vec_.resize(length);
}

void Terminal::addProtocol(ProtocolID pid, Protocol* protocol)
{
    delProtocol(pid);
    protocol->init(pid);
    protocol_map_.set(pid, protocol);
    KIT_SAFE_RETAIN(protocol);
}

void Terminal::delProtocol(ProtocolID pid)
{
    protocol_map_.del(pid, [](Protocol* p){
        KIT_SAFE_RELEASE(p);
    });
}

Protocol* Terminal::getProtocol(ProtocolID pid) const
{
    Protocol* protocol = NULL;
    protocol_map_.get(pid, protocol);
    return protocol;
}

Session* Terminal::getSession(SessionID sid) const
{
    Session* session = NULL;
    session_map_.get(sid, session);
    return session;
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

void Terminal::sendProtocol(SessionID sid, const Protocol* protocol)
{
    Session* session = getSession(sid);
    if (session == NULL)
    {
        ERR("[Terminal](sendProtocol) no session:%d", sid);
        return;
    }
    sessionSendProtocol(session, protocol);
}

void Terminal::sessionSendProtocol(Session* session, const Protocol* protocol)
{
    Packet* pack = Packet::create(false);
    Buffer* buf = g_BufPool->createBuffer(protocol->getBudgetSize());
    protocol->serialize(buf);
    pack->init(protocol->getPID(), buf);
    buf->release();
    sessionSendPacket(session, pack);
    pack->release();
}

void Terminal::sessionSendPacket(Session* session, Packet* packet)
{
    bool clear_out = session->sendPacket(packet);
    if (!clear_out)
        out_vec_.push_back(session);
}

void Terminal::recvProtocol(SessionID sid, const Protocol* protocol)
{
    DBG(protocol->toString().c_str());
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


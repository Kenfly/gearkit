#ifndef __KIT_TERMINAL_H__
#define __KIT_TERMINAL_H__

#include "netsys.h"
#include "Ref.h"
#include "Map.h"

#ifndef SESSION_LENGTH
#define SESSION_LENGTH 1024
#endif

#ifndef PROTOCOL_LENGTH
#define PROTOCOL_LENGTH 512
#endif

namespace kit {

class Session;
class Protocol;
class PacketHandler;

class Terminal : public Ref
{
public:
	Terminal();
    virtual ~Terminal();

    virtual void handleSession(Session* session);

    virtual void sendProtocol(SessionID sid, const Protocol* pto);
    virtual void recvProtocol(SessionID sid, const Protocol* pto);

    Protocol* getProtocol(ProtocolID pid) const;
    void addProtocol(ProtocolID pid, Protocol* pto);
    void delProtocol(ProtocolID pid);

    Session* getSession(SessionID sid) const;
    void addSession(Session* session);
    void delSession(SessionID sid);

    void clearProtocols();
    void clearSessions();
protected:
    virtual bool baseInit();

    typedef Map<SessionID, Session*, SESSION_LENGTH> SessionMap;
    SessionMap session_map_;

    typedef Map<ProtocolID, Protocol*, PROTOCOL_LENGTH> ProtocolMap;
    ProtocolMap protocol_map_;
};

} // namespcae kit

#endif


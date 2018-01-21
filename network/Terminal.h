#ifndef __KIT_TERMINAL_H__
#define __KIT_TERMINAL_H__

#include "netsys.h"
#include "Ref.h"
#include "Map.h"

#ifndef SESSION_LENGTH
#define SESSION_LENGTH 1024
#endif

namespace kit {

class Socket;
class Session;
class Protocol;
class PacketHandler;

class Terminal : public Ref
{
public:
	Terminal();
    virtual ~Terminal();

    virtual PacketHandler* getPacketHandler() const { return packet_handler_; }
    virtual void setPacketHandler(PacketHandler* handler);

    virtual void handleRecvPackets(Socket* sock);

    //virtual void sendProtocol(SessionID sid, const Protocol* pto);

protected:
    virtual bool baseInit();

    Session* getSession(SessionID sid) const;
    SessionID addSession(Socket* sock);


    PacketHandler* packet_handler_;

    typedef Map<SessionID, Session*, SESSION_LENGTH> SessionMap;
    SessionMap session_map_;
};

} // namespcae kit

#endif

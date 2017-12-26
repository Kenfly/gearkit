#ifndef __KIT_TERMINAL_H__
#define __KIT_TERMINAL_H__

#include "Ref.h"

namespace kit {

class Socket;
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
protected:
    virtual bool baseInit();


    PacketHandler* packet_handler_;
};

} // namespcae kit

#endif

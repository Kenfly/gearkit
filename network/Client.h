#ifndef __KIT_CLIENT_H__
#define __KIT_CLIENT_H__

#include "Ref.h"
#include "kitsys.h"
#include "HashList.h"

namespace kit {

class Socket;
class Buffer;

class IClient : public Ref
{
public:
	IClient();
    virtual ~IClient();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 每帧调用
    virtual void update() = 0;

    void sendPacket(Buffer* buf);
protected:
	Socket* socket_;
    IPacketHandler* packet_handler_;
    bool active_;
};

} // namespcae kit

#ifdef PLATFORM_LINUX
#include <platform/linux/Client_linux.h>
#elif defined PLATFORM_WINDOWS
#include <platform/win/Client_win.h>
#endif

#endif

#ifndef __KIT_SOCKET_LINUX_H__
#define __KIT_SOCKET_LINUX_H__

#include "Socket.h"

namespace kit {

class Socket : public ISocket
{
public:
    KIT_CREATE_FUNC(Socket)
};

} // namespace kit

#endif


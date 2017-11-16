#ifndef __KIT_SOCK_ADDR_LINUX_H__
#define __KIT_SOCK_ADDR_LINUX_H__

#include "SockAddr.h"

namespace kit {

class SockAddr : public ISockAddr
{
public:
    KIT_CREATE_FUNC(SockAddr)
    virtual ~SockAddr();
    virtual void init(const char* ip, uint16_t port, uint16_t family);
};

} // namespace kit

#endif


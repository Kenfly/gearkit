#ifndef __KIT_SOCK_ADDR_H__
#define __KIT_SOCK_ADDR_H__

#include "Ref.h"
#include "kitsys.h"
#include <string>

struct sockaddr;
struct sockaddr_in;

namespace kit {

class ISockAddr : public Ref
{
public:
    ISockAddr();
    virtual ~ISockAddr();
    virtual void init(const char* ip, uint16_t port, uint16_t family);
    bool valid() const { return valid_; }

    operator sockaddr_in*() { return (sockaddr_in*)addr_; }
    operator sockaddr*() { return (sockaddr*)addr_; }
public:
    std::string ip;
    uint16_t port;
protected:
    sockaddr* addr_;
    bool valid_;
};

} // namespace kit

#ifdef PLATFORM_LINUX
#include <platform/linux/SockAddr_linux.h>
#elif defined PLATFORM_WINDOWS
#include <platform/win/SockAddr_win.h>
#endif

#endif


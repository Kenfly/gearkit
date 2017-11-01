#ifndef __KIT_SOCK_ADDR_H__
#define __KIT_SOCK_ADDR_H__

#include "netbase.h"

struct sockaddr;
struct sockaddr_in;

namespace kit {

class SockAddr
{
public:
    SockAddr();
    ~SockAddr();
    void init(const char* ip, uint16_t port, uint16_t family);
    bool valid() { return valid_; }

    operator sockaddr_in*() { return (sockaddr_in*)addr_; }
    operator sockaddr*() { return (sockaddr*)addr_; }
protected:
    sockaddr* addr_;
    bool valid_;
};

} // namespace kit
#endif

#include "SockAddr.h"
#include "base.h"
#include <netinet/in.h>

namespace kit {

SockAddr::SockAddr()
: addr_(NULL)
{
}

SockAddr::~SockAddr()
{
    if (addr_ != NULL)
    {
        delete addr_;
    }
}

void SockAddr::init(const char* ip, uint16_t port, uint16_t family)
{
    if (addr_ == NULL)
    {
        addr_ = new sockaddr();
    }
    memset(addr_, 0, sizeof(*addr_));

    struct sockaddr_in *addr = (struct sockaddr_in*)addr_;

    //判断ip是域名还是ip地址
    bool is_name = false;
    for (int i = 0; ip[i]; ++i)
    {
        char s = ip[i];
		if (!((s >= '0' && s <= '9') || s == '.'))
        {
			is_name = true;
			break;
		}
    }

    if (is_name)
    {
        struct hostent* ht = ::gethostbyname(ip);
        if (ht == NULL || ht->h_length != 4)
        {
            ERR("[SockAddr](init) error! %s, %d", __FILE__, __LINE__);
            return;
        }

        memcpy((char*)&(addr->sin_addr), ht->h_addr, ht->h_length);
    }
    else
    {
        addr->sin_addr.S_un.S_addr = inet_addr(ip);
    }

    addr->sin_port = htons(port);
    addr->sin_family = family;
}

} // namespace kit


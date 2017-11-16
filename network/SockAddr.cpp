#include "SockAddr.h"

namespace kit {

ISockAddr::ISockAddr()
: addr_(NULL)
, valid_(false)
, ip("")
, port(0)
{
}

ISockAddr::~ISockAddr()
{
}

void ISockAddr::init(const char* ip, uint16_t port, uint16_t family)
{
    this->ip = ip;
    this->port = port;
    // need realize
    valid_ = true;
}

} // namespace kit


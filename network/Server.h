#ifndef __KIT_SERVER_H__
#define __KIT_SERVER_H__

#include "netbase.h"

namespace kit {

const uint16_t CONNECTION_LIMIT = 0xFFFF;

class Socket;

class Server
{
public:
	Server();
    ~Server();
	void startup(const char* ip, int32_t port);
	void shutdown();
private:
	Socket* socket_;

	//Socket* socket_list_[CONNECTION_LIMIT];
};

} // namespcae kit

#endif

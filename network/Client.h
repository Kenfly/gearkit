#ifndef __KIT_CLIENT_H__
#define __KIT_CLIENT_H__

#include "Ref.h"
#include "kitsys.h"
#include "HashList.h"

namespace kit {

class Socket;

class IClient : public Ref
{
public:
	IClient();
    virtual ~IClient();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 每帧调用
    virtual void update() = 0;

    inline int32_t getTimeval() const { return timeval_; }
    inline void setTimeval(int32_t timeval) { timeval_ = timeval; }
protected:
	Socket* socket_;
    bool active_;

    // wait time
    int32_t timeval_;
};

} // namespcae kit

#ifdef PLATFORM_LINUX
#include <platform/linux/Client_linux.h>
#elif defined PLATFORM_WINDOWS
#include <platform/win/Client_win.h>
#endif

#endif

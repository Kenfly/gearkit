#ifndef __KIT_THREAD_H__
#define __KIT_THREAD_H__

#include "kitsys.h"
#include "Ref.h"

namespace kit {

class IMutex;
// 线程函数
struct ThreadHandler
{
    virtual void handleThread() = 0;
};

class  IThread : public Ref
{
public:
    IThread();
    virtual ~IThread();

    virtual bool baseInit();
    virtual void init(ThreadHandler* handler);
    virtual void setHandler(ThreadHandler* handler);

    // 等待线程结束
    virtual void join() = 0;
    virtual int32_t start() = 0;

    virtual void stop();
    virtual void run();
protected:
    bool active_;
    IMutex* mutex_;
    ThreadHandler* handler_;
};


} // namepsace kit

#ifdef PLATFORM_LINUX
#include "platform/linux/Thread_linux.h"
#elif defined PLATFORM_WINDOWS
#include "platform/win/Thread_win.h"
#endif

#endif


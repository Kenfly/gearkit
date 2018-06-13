#ifndef __KIT_THREAD_LINUX_H__
#define __KIT_THREAD_LINUX_H__

#include "Thread.h"
#include <pthread.h>

namespace kit {

class Thread : public IThread
{
public:
    KIT_CREATE_FUNC(Thread)

    Thread();
    virtual ~Thread();

    virtual void join();
    virtual int32_t start();
private:
    pthread_t pid_;
};

} // namespace kit

#endif


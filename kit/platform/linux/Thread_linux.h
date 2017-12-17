#ifndef __KIT_THREAD_LINUX_H__
#define __KIT_THREAD_LINUX_H__

#include "Thread.h"
#include <pthread.h>

namespace kit {

class Mutex : public IMutex
{
public:
    KIT_CREATE_FUNC(Mutex)

    Mutex();
    virtual ~Mutex();

    virtual int32_t lock();
    virtual int32_t unlock(); 
    virtual int32_t tryLock();
protected:
    virtual bool baseInit();

    pthread_mutex_t mutex_;
};

class SpinMutex : public IMutex
{
public:
    KIT_CREATE_FUNC(SpinMutex)

    SpinMutex();
    virtual ~SpinMutex();

    virtual int32_t lock();
    virtual int32_t unlock(); 
    virtual int32_t tryLock();
protected:
    virtual bool baseInit();

    pthread_spinlock_t mutex_;
};


/////////////////////////////////////////

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


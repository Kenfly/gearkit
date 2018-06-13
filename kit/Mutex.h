#ifndef __KIT_MUTEX_H__
#define __KIT_MUTEX_H__

#include "kitsys.h"
#include "Ref.h"

namespace kit {

// 锁
class IMutex : public Ref
{
public:
    IMutex() {}
    virtual ~IMutex() {}

    virtual int32_t lock() = 0;
    virtual int32_t unlock() = 0;
    virtual int32_t tryLock() = 0;
};

class Mutex;
class SpinMutex;

class AutoLock
{
public:
    AutoLock(IMutex* mutex);
    ~AutoLock();
protected:
    IMutex* mutex_;
};

} // namespace kit

#ifdef PLATFORM_LINUX
#include "platform/linux/Mutex_linux.h"
#elif defined PLATFORM_WINDOWS
#include "platform/win/Mutex_win.h"
#endif

#endif



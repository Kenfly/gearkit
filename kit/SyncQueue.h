#ifndef __KIT_SYNC_QUEUE_H__
#define __KIT_SYNC_QUEUE_H__

#include "Queue.h"
#include "Mutex.h"

namespace kit {

template<typename Type, const int Length>
class SyncQueue : public Queue<Type, Length>
{
public:
    SyncQueue()
    {
        pop_mutex_ = Mutex::create(false);
        push_mutex_ = Mutex::create(false);
    }

    virtual ~SyncQueue()
    {
        KIT_SAFE_RELEASE(pop_mutex_);
        KIT_SAFE_RELEASE(push_mutex_);
    }

    bool pop(Type& value)
    {
        AutoLock _(pop_mutex_);
        return Queue<Type, Length>::pop(value);
    }

    bool push(const Type& value)
    {
        AutoLock _(push_mutex_);
        return Queue<Type, Length>::push(value);
    }
private:
    // limit
    bool front(Type& value) { return false; }
    int rest() { return 0; }
    int count() const { return 0; }

    Mutex* pop_mutex_;
    Mutex* push_mutex_;
};

} // namespace kit

#endif


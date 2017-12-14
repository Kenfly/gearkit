#ifndef __KIT_QLIST_H__
#define __KIT_QLIST_H__

// que + list
// que first. list second
//
#include "Queue.h"
#include "List.h"

namespace kit {

template<typename Type, const int QLength>
class QList
{
public:
    QList(): count_(0)
    {
    }

    bool pop(Type& value)
    {
        bool ret = que_.pop(value) || list_.pop(value);
        if (ret)
            --count_;
        return ret;
    }

    bool push(const Type& value)
    {
        if (list_.count() > 0 || !que_.push(value))
            list_.push(value);
        ++count_ ;
        return true;
    }

    bool front(Type& value)
    {
        return que_.front(value) || list_.front(value);
    }

    int count() const
    {
        return count_;
    }

private:
    int count_;
    Queue<Type, QLength> que_;
    List<Type> list_;
};

} // namespac kit

#endif


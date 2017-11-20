#ifndef __KIT_QUEUE_H__
#define __KIT_QUEUE_H__

namespace kit {

template<typename Type, const int Length>
class Queue
{
public:
    Queue(): count_(0), head_(0), tail_(0)
    {
    }

    bool pop(Type& value)
    {
        if (count_ == 0)
            return false;
        value = que_[head_];
        ++head_;
        head_ %= Length;
        --count_;

        return true;
    }

    bool push(const Type& value)
    {
        if (count_ == Length)
            return false;
        que_[tail_] = value;
        ++tail_;
        tail_ %= Length;
        ++count_;

        return true;
    }

    bool front(Type& value)
    {
        if (count_ == 0)
            return false;
        value = que_[head_];
        return true;
    }

    int count()
    {
        return count_;
    }

    int rest()
    {
        return Length - count_;
    }

private:
    int count_;
    int head_;
    int tail_;
    Type que_[Length];
};

} // namespace kit

#endif


#ifndef __KIT_QUEUE_H__
#define __KIT_QUEUE_H__

namespace kit {

// normal queue
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

    int count() const
    {
        return count_;
    }

    int rest() const
    {
        return Length - count_;
    }

private:
    int count_;
    int head_;
    int tail_;
    Type que_[Length];
};


// priority queue
template<typename Type, const int Length>
class MinQueue
{
public:
    MinQueue(): count_(0)
    {
    }

    bool pop(Type& value)
    {
        if (count_ == 0)
            return false;
        value = que_[1];
        que_[1] = que_[count_];
        --count_;
        adjustDown(1);
        return true;
    }

    bool push(const Type& value)
    {
        if (count_ == Length)
            return false;
        ++count_;
        que_[count_] = value;
        adjust(count_);
        return true;
    }

    int count() const { return count_; }
    int rest() const { return Length + 1 - count_; }
protected:
    virtual bool compare(const Type& a, const Type& b) const
    {
        return a > b;
    }

    bool adjustDown(int index)
    {
        int left;
        int right;
        int target;
        bool changed = false;
        while (index < count_)
        {
            left = index << 1;
            right = left + 1;
            target = left;
            if (right <= count_ && compare(que_[left], que_[right]))
            {
                target = right;
            }
            if (target <= count_ && compare(que_[index], que_[target]))
            {
                exchange(index, target);
                index = target;
                changed = true;
            }
            else
                break;
        }
        return changed;
    }

    void adjust(int index)
    {
        while ((index >>= 1) > 0)
        {
            if (! adjustDown(index))
                break;
        }
    }

    void exchange(int index1, int index2)
    {
        Type tmp = que_[index1];
        que_[index1] = que_[index2];
        que_[index2] = tmp;
    }
private:
    int count_;
    Type que_[Length + 1];
};

template<typename Type, const int Length>
class MaxQueue : public MinQueue<Type, Length>
{
    protected:
        virtual bool compare(const Type&a, const Type& b) const
        {
            return a < b;
        }
};

} // namespace kit

#endif


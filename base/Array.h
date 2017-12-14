#ifndef __KIT_ARRAY_H__
#define __KIT_ARRAY_H__

// 方便遍历的数组

#include <stdio.h>
#include <memory.h>

namespace kit {

// item.next与item.prev都为0标志为空
// next, prev为负数为空

template<typename Type, const int Length>
class Array
{
    struct Item
    {
        int next;
        int prev;
        Type value;
    };
public:
    Array() : count_(0), head_(-1), tail_(-1)
    {
        clear();
    }

    bool get(int index, Type& val)
    {
        Item& item = array_[index];
        if (itemEmpty(item))
            return false;
        val = item.value;
        return true;
    }

    void set(int index, const Type& val)
    {
        Item& item = array_[index];
        if (itemEmpty(item))
        {
            item.prev = tail_;
            item.value = val;
            item.next = -1;
            if (head_ == -1)
                head_ = index;

            if (tail_ != -1)
                array_[tail_].next = index;
            tail_ = index;
            ++count_;
        }
        else
        {
            item.value = val;
        }
    }

    void del(int index)
    {
        Item& item = array_[index];
        if (itemEmpty(item))
            return;
        int next = item.next;
        int prev = item.prev;
        if (prev != -1)
            array_[prev].next = next;
        if (next != -1)
            array_[next].prev = prev;
        if (head_ == index)
            head_ = next;
        if (tail_ == index)
            tail_ = prev;
        emptyItem(item);
        --count_;
    }
    int count() const { return count_; }

    void clear()
    {
        memset(array_, 0, sizeof(Item) * Length);
        head_ = tail_ = -1;
        count_ = 0;
    }

private:
    bool itemEmpty(const Item& item) const
    {
        return item.next == 0 && item.prev == 0;
    }
    void emptyItem(Item& item)
    {
        item.next = item.prev = 0;
    }
    int count_;
    int head_;
    int tail_;
    int pos_;
    Item array_[Length];

public:
    // iterator
    class Iterator
    {
    public:
        Iterator(Item* array, int index) 
        : array_(array)
        , index_(index)
        {
        }
        Iterator& operator ++()
        {
            index_ = array_[index_].next;
            return *this;
        }
        bool operator ==(const Iterator& it)
        {
            return index_ == it.index_ ;
        }
        bool operator !=(const Iterator& it)
        {
            //printf("%d %d\n", index_, it.index_);
            return index_ != it.index_;
        }
        Type& operator *()
        {
            return array_[index_].value;
        }
    private:
        Item* array_;
        int index_;
    };

    Iterator begin()
    {
        return Iterator(array_, head_);
    }
    Iterator end()
    {
        return Iterator(array_, -1);
    }
};

} // namespace kit

#endif


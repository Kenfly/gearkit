#ifndef __KIT_ARRAY_H__
#define __KIT_ARRAY_H__

// 方便遍历的数组

namespace kit {

// item.next为-2标志为空
// next, prev为负数为空

template<typename Type, const int Length>
class Array
{
    struct Item
    {
        Item(): next(-2) {}
        int next;
        int prev;
        Type value;
    };
public:
    Array() : count_(0), head_(-1), tail_(-1), pos_(-1)
    {
    }

    bool get(int index, Type& val)
    {
        Item& item = array_[index];
        if (item.next == -2)
            return false;
        val = item.value;
    }

    void add(int index, const Type& val)
    {
        Item& item = array_[index];
        if (item.next == -2)
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
        int next = item.next;
        int prev = item.prev;
        if (next == -2)
            return;
        if (prev != -1)
            array_[prev].next = next;
        if (next != -1)
            array_[next].prev = prev;
        if (head_ == index)
            head_ = next;
        if (tail_ == index)
            tail_ = prev;
        item.next = -2;
        item.prev = -1;
        --count_;
    }
    int count() const { return count_; }

    void resetNext() { pos_ = head_; }

    bool next(Type& val)
    {
        if (pos_ < 0)
            return false;
        int cur = pos_;
        pos_ = array_[cur].next;
        get(cur, val);
        return get(cur, val);
    }
    void clear()
    {
        int pos = head_;
        while (pos >= 0)
        {
            Item& item = array_[pos];
            pos = item.next;
            item.next = -2;
            item.prev = -1;
        }
        head_ = tail_ = -1;
        count_ = 0;
    }

private:
    int count_;
    int head_;
    int tail_;
    int pos_;
    Item array_[Length];
};

} // namespace kit

#endif


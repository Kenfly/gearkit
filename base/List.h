#ifndef __KIT_LIST_H__
#define __KIT_LIST_H__

namespace kit {

template<typename Type>
class List
{
    struct Item 
    {
        Type value;
        Item* next;
        Item* prev;
    };
public:
    List(): head_(0), tail_(0), count_(0)
    {
    }
    int count() { return count_; }

    bool front(Type& value)
    {
        if (count_ == 0 || head_ == 0)
            return false;
        value = head_->value;
        return true;
    }

    // 从头删除
    bool pop(Type& value)
    {
        if (count_ == 0 || head_ == 0)
            return false;
        value = head_->value;
        Item* nxt = head_->next;
        if (nxt)
            nxt->prev = 0;
        else
            tail_ = 0;
        delete head_;
        head_ = nxt;
        --count_;
        return true;
    }

    // 从尾添加
    bool push(const Type& item)
    {
        Item* p = new Item();
        p->value = item;
        p->next = 0;
        p->prev = tail_;
        if (tail_)
            tail_->next = p;
        if (head_ == 0)
            head_ = p;
        tail_ = p;
        ++count_;
        return true;
    }

    // TODO: iterator
    // TODO: 
    bool insert(int index, const Type& item)
    {
        /*
        if (count_ < index)
        {
            push(item);
            return true;
        }

        Item* p = head_;
        for (int i = 0; true; ++i)
        {
        }
        */
    }

    bool erase(int index)
    {
    }
private:
    Item* head_;
    Item* tail_;
    int count_;
};

}

#endif


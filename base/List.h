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
    ~List()
    {
        clear();
    }

    int count() const { return count_; }

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

    void clear()
    {
        Item* p = head_;
        Item* pn = 0;
        while (p)
        {
            pn = p->next;
            delete p;
            p = pn;
        }
    }

    // TODO: insert
    bool insert(int index, const Type& item)
    {
        return true;
    }

    // TODO: erase
    bool erase(int index)
    {
        return true;
    }

private:
    Item* head_;
    Item* tail_;
    int count_;
public:
    // iterator
    class Iterator
    {
    public:
        Item* node_;
        Iterator(Item* node) : node_(node)
        {
        }
        Iterator& operator ++()
        {
            node_ = node_->next;
            return *this;
        }
        bool operator ==(const Iterator& it)
        {
            return node_ == it.node_;
        }
        bool operator !=(const Iterator& it)
        {
            return node_ != it.node_;
        }
        Type& operator *()
        {
            return node_->value;
        }
    };
    Iterator begin() const
    {
        return Iterator(head_);
    }
    Iterator end() const
    {
        return Iterator(NULL);
    }
};

}

#endif


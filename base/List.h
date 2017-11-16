#ifndef __KIT_LIST_H__
#define __KIT_LIST_H__

namespace kit {

class IListItem
{
public:
    IListItem* nextListItem__;
    IListItem* prevListItem__;
};

class List
{
public:
    int count() { return count_; }
private:
    IListItem* head_;
    IListItem* tail_;
    int count_;
};

}

#endif


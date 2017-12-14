#ifndef __KIT_MAP_H__
#define __KIT_MAP_H__

/*
 * 没有iterator
 */

#include <memory.h>

namespace kit {

template<typename KeyType, typename ValueType, const int Length>
class Map
{

    typedef struct Item
    {
        Item(KeyType k, const ValueType& v, Item* n): key(k), value(v), next(n) {}
        KeyType key;
        ValueType value;
        Item* next;
    } *ItemList;

public:
    Map()
    {
        memset(vec_, 0, sizeof(ItemList) * Length);
    }
    ~Map()
    {
        clear();
    }
    bool get(KeyType key, ValueType& v) const
    {
        int index = key % Length;
        ItemList plist = vec_[index];
        if (plist == 0)
            return false;
        while (plist)
        {
            if (plist->key == key)
            {
                v = plist->value;
                return true;
            }
            plist = plist->next;
        }
        return false;
    }
    void set(KeyType key, const ValueType& v)
    {
        int index = (key % Length);
        ItemList plist = vec_[index];
        ItemList pprev = 0;
        while (plist)
        {
            if (plist->key == key)
                break;
            pprev = plist;
            plist = plist->next;
        }
        if (plist == 0)
        {
            plist = new struct Item(key, v, 0);
        }
        if (pprev)
            pprev->next = plist;
        else
            vec_[index] = plist;
    }
    bool del(KeyType key)
    {
        int index = (key % Length);
        ItemList plist = vec_[index];
        if (plist == 0)
            return false;
        ItemList pprev = 0;
        while (plist)
        {
            if (plist->key == key)
                break;
            pprev = plist;
            plist = plist->next;
        }
        if (plist)
        {
            if (pprev)
                pprev->next = plist->next;
            else
                vec_[index] = 0;
            delete plist;
            return true;
        }
        return false;
    }
    void clear()
    {
        ItemList plist = 0;
        ItemList pnext = 0;
        for (int i = 0; i != Length; ++i)
        {
            plist = vec_[i];
            while (plist)
            {
                pnext = plist->next;
                delete plist;
                plist = pnext;
            }
            vec_[i] = 0;
        }
    }
private:
    ItemList vec_[Length];
};

} // namespace kit

#endif


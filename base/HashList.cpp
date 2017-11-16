#include "HashList.h"
#include "Ref.h"

namespace kit {

void HashList_init(HashListT hl, int size)
{
    memset(hl, 0, size * sizeof(HLElement));
    hl[0].u.count[1] = size;
}

void HashList_addElement(HashListT hl, int index, Ref* p)
{
    assert(index > 0 && index < HashList_length(hl));

    p->retain();

    HLElement& element = hl[index];
    HLElement& info = hl[0];
    if (element.u.ref != 0)
    {
        element.u.ref = p;
    }
    else
    {
        element.u.ref = p;

        int head = info.prev;
        int tail = info.next;
        if (head == 0)
        {
            info.prev = info.next = index;
        }
        else
        {
            HLElement& tail_element = hl[tail];
            tail_element.next = info.next = index;

            element.prev = tail;
        }
        ++info.u.count[0];
    }
}

// 删除元素
void HashList_delElement(HashListT hl, int index)
{
    assert(index > 0 && index < HashList_length(hl));

    HLElement& element = hl[index];

    Ref* ref = element.u.ref;
    if (ref == 0)
        return;

    HLElement& info = hl[0];
    int prev = element.prev;
    int next = element.next;
    if (prev == 0 and next == 0)
    {
        //只有这一个元素
        info.prev = info.next = 0;
    }
    else if (prev == 0)
    {
        //第一个元素
        info.prev = next;
        hl[next].prev = 0;
    }
    else if (next == 0)
    {
        //最后一个元素
        info.next = prev;
        hl[prev].next = 0;
    }
    else
    {
        //中间元素
        hl[prev].next = next;
        hl[next].prev = prev;
    }

    memset(&element, 0, sizeof(HLElement));

    --info.u.count[0];

    ref->release();
}

// debug
void HashList_print(HashListT hl)
{
    printf("length:%d,count:%d: [", HashList_length(hl), HashList_count(hl));
    int index = 0;
    while (index = kit::HashList_next(hl, index))
    {
        printf("%d,", index);
    }
    printf("]\n");
}


} // namespace kit


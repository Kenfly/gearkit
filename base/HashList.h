#ifndef __KIT_HASH_LIST_H__
#define __KIT_HASH_LIST_H__

#include <assert.h>
#include <memory.h>
#include <stdio.h>

/**
 *1.头部存储count, head, tail
 *2.hash key只能为数字，并且从1开始
 *
 *
 **/

namespace kit {

class Ref;

typedef struct HLElement
{
    union
    {
        Ref* ref;
        int count[2]; //0记录长度，1记录数组长度
    }u;
    int prev;
    int next;
} HashListElement, HashListDef, *HashListT;


// 初始化
void HashList_init(HashListT hl, int size);

// 已有元素个数
inline int HashList_count(HashListT hl)
{
    return hl[0].u.count[0];
}

// 容器大小
inline int HashList_length(HashListT hl)
{
    return hl[0].u.count[1];
}

// 获取元素
inline Ref* HashList_getElement(HashListT hl, int index)
{
    assert(index > 0 && index < HashList_length(hl));

    return hl[index].u.ref; 
}

// 遍历
inline int HashList_next(HashListT hl, int index)
{
    assert(index >= 0 && index < HashList_length(hl));

    const HLElement& element = hl[index];
    if (index == 0)
        return hl[0].prev;
    else
        return hl[index].next;
}

// 添加元素
void HashList_addElement(HashListT hl, int index, Ref* p);

// 删除元素
void HashList_delElement(HashListT hl, int index);

// debug
void HashList_print(HashListT hl);

} // namespace kit

#endif


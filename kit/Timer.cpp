#include "Timer.h"
#include <memory.h>
#include <stdio.h>

namespace kit {

Timer::Wheel::Wheel()
{
    memset(nodes, 0, sizeof(Node*));
}

Timer::Wheel::~Wheel()
{
    Node* p;
    Node* np;
    for (int i = 0; i != 512; ++i)
    {
        p = nodes[i];
        while (p)
        {
            np = p->next;
            delete p;
            p = np;
        }
    }
}

Timer::Timer()
: id_(0)
{
}

Timer::~Timer()
{
}

void Timer::update()
{
    wheeling(0);
}

uint32_t Timer::addTimer(uint32_t delay, TimerHandler handler, uint32_t cycle)
{
    Node* node = new Node(++id_, delay, cycle);
    addNode(3, node);

    handler_map_.set(id_, handler);
    return id_;
}

void Timer::delTimer(uint32_t id)
{
    handler_map_.del(id);
}

void Timer::wheeling(uint8_t index)
{
    Wheel& wheel = wheels_[index];
    ++wheel.pos;

    Node* np;
    bool ret;
    Node* p = wheel.nodes[wheel.pos];
    while (p)
    {
        np = p->next;
        if (!addNode(index - 1, p))
        {
            ret = handleTimer(p->id);
            if (!ret && p->cycle > 0)
            {
                p->delay = p->cycle;
                addNode(3, p);
            }
        }
        p = np;
    }
    wheel.nodes[wheel.pos] = NULL;

    // 拉动下一个轮盘
    if (wheel.pos == 0 && index < 3)
    {
        wheeling(index + 1);
    }
}

bool Timer::addNode(uint8_t index, Node* node)
{
    if (index > 3)
        return false;
    Wheel& wheel = wheels_[index];
    uint8_t pos = node->bits[index];
    if (pos > 0)
    {
        pos = wheel.pos + pos;
        node->next = wheel.nodes[pos];
        wheel.nodes[pos] = node;
        return true;
    }
    else
        return addNode(index - 1, node);
}

bool Timer::handleTimer(uint32_t id)
{
    TimerHandler handler;
    if (handler_map_.get(id, handler))
    {
        return handler(id);
    }
    return true;
}

} // namespace kit


#ifndef __KIT_TIMER_H__
#define __KIT_TIMER_H__

#include "kitsys.h"
#include "Ref.h"
#include "Map.h"
#include <functional>

#ifndef TIMER_MAP_SIZE
#define TIMER_MAP_SIZE (512)
#endif

namespace kit {

typedef std::function<bool(uint32_t)> TimerHandler;

class Timer : public Ref
{
    struct Node 
    {
        Node(uint32_t _id, uint32_t _delay, uint32_t _cycle = 0)
        : id(_id)
        , delay(_delay)
        , cycle(_cycle)
        , next(NULL)
        {
        }

        uint32_t id;
        union {
            uint32_t delay;
            uint8_t bits[4];
        };
        uint32_t cycle;

        Node* next;
    };

    struct Wheel
    {
        Wheel();
        ~Wheel();
        uint8_t pos;
        Node* nodes[512];
    };

public:
    KIT_CREATE_FUNC(Timer)
    Timer();
    virtual ~Timer();

    // cycle > 0循环
    uint32_t addTimer(uint32_t delay, TimerHandler handler, uint32_t cycle = 0);
    void delTimer(uint32_t id);

    void update();

private:
    // 旋转某个轮盘
    void wheeling(uint8_t index = 0);

    // 返回true结束循环
    bool handleTimer(uint32_t id);
    bool addNode(uint8_t index, Node* node);
private:
    uint32_t id_;
    Wheel wheels_[4];
    Map<uint32_t, TimerHandler, TIMER_MAP_SIZE> handler_map_;
};

} // namespace kit

#endif


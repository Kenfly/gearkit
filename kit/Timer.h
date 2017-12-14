#ifndef __KIT_TIMER_H__
#define __KIT_TIMER_H__

#include "kitsys.h"
#include "Ref.h"

namespace kit {


// 处理器
struct TimerHandler
{
    virtual bool handleTimer(uint32_t id) = 0;
};

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

    virtual void init(TimerHandler* handler);
    // cycle > 0循环
    uint32_t addTimer(uint32_t delay, uint32_t cycle = 0);
    // TODO: dont call this
    void delTimer(uint32_t id);

    void update();

    // 设置handler， 一个timer只有一个
    void setHandler(TimerHandler* handler);
private:
    // 旋转某个轮盘
    void wheeling(uint8_t index = 0);

    // 返回true结束循环
    bool handleTimer(uint32_t id);
    bool addNode(uint8_t index, Node* node);
private:
    uint32_t id_;
    Wheel wheels_[4];
    TimerHandler* handler_;
};

} // namespace kit

#endif


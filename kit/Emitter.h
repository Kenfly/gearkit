#ifndef __KIT_EMITTER_H__
#define __KIT_EMITTER_H__

/*
 *
 *  event emitter
 *
 */

#include "Ref.h"
#include <vector>
#include <functional>

#ifndef DEFAULT_EVNET_LENGTH
#define DEFAULT_EVNET_LENGTH 16
#endif

namespace kit {

class Event : public Ref
{
public:
    int event_id;
};

template<int EventLength = 16>
class Emitter
{
public:
    typedef int HandlerID;
    typedef std::function<void (const Event*)> Handler;

    typedef struct 
    {
        HandlerID id;
        Handler handler;
        bool once;
        bool deleted;
    } HandlerItem;

public:
    Emitter()
    : id_(0)
    {
    }
    ~Emitter()
    {
        clear();
    }

    HandlerID on(int event_id, Handler handler)
    {
        handler_vecs_[event_id].push_back({++id_, handler, false, false});
        return id_;
    }

    void once(int event_id, Handler handler)
    {
        handler_vecs_[event_id].push_back({++id_, handler, true, false});
    }

    void off(int event_id, HandlerID id)
    {
        HandlerVec& vec = handler_vecs_[event_id];
        int len = vec.size();
        for (int i = 0;  i != len; ++i)
        {
            if (vec[i].id == id)
            {
                vec[i].deleted = true;
                return;
            }
        }
    }

    void emit(int event_id, const Event* event)
    {
        HandlerVec& vec = handler_vecs_[event_id];
        int len = vec.size();
        int tail = len;

        for (int i = 0; i != tail;)
        {
            auto& item = vec[i];
            if (!item.deleted)
            {
                item.handler(event);
                if (item.once)
                    item.deleted = true;
            }
            if (item.deleted)
                __swap(vec, i, --tail);
            else
                ++i;
        }
        if (len == vec.size())
        {
            vec.resize(tail);
        }
    }

    // don't call this in handlers
    void clear()
    {
        for (int i = 0; i != EventLength; ++i)
        {
            handler_vecs_[i].clear();
        }
    }
private:
    typedef std::vector<HandlerItem> HandlerVec;

    void __swap(HandlerVec& vec, int ia, int ib)
    {
        const HandlerItem& temp = vec[ia];
        vec[ia] = vec[ib];
        vec[ib] = temp;
    }

    HandlerID id_;
    HandlerVec handler_vecs_[EventLength];
};

} // namespace kit

// call this in class, should be public scope
#define EMITTER(args...) \
    enum { args, __EVENT_MAX__, }; \
    kit::Emitter<__EVENT_MAX__ < DEFAULT_EVNET_LENGTH ? DEFAULT_EVNET_LENGTH : __EVENT_MAX__> ___emitter___;

// call this in class
#define EMIT(__EVENT_TYPE__, __EVENT__) \
    ___emitter___.emit(__EVENT_TYPE__, __EVENT__);

// call this outside class
#define SENDER_EMIT(__SENDER__, __EVENT_TYPE__, __EVENT__) \
    __SENDER__.___emitter___.emit(__EVENT_TYPE__, __EVENT__);

// call this outside class
#define SLOT(__SENDER__, __EVENT_TYPE__, __HANDLER__) \
    __SENDER__.___emitter___.on(__EVENT_TYPE__, __HANDLER__);

#define UNSLOT(__SENDER__, __EVENT_TYPE__, __HANDLER_ID__) \
    __SENDER__.___emitter___.off(__EVENT_TYPE__, __HANDLER_ID__);

#define SLOT_ONCE(__SENDER__, __EVENT_TYPE__, __HANDLER__) \
    __SENDER__.___emitter___.once(__EVENT_TYPE__, __HANDLER__);




#endif


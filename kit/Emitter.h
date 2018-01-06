#ifndef __KIT_EMITTER_H__
#define __KIT_EMITTER_H__

/*
 *
 *  event emitter
 *
 */

#include "Ref.h"
#include <functional>
#include <unordered_map>

namespace kit {

class Event : public Ref
{
};

class Emitter : public Ref
{
public:
    Emitter();
    ~Emitter();

    void on();
    void off();

    void emit(const std::string& ev);

private:
    typdef std::vector<Event*> EventList;
    std::unordered_map<std::string, EventList> event_map_;
};

} // namespace kit

#endif


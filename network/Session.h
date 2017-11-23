#ifndef __KIT_SESSION_H__
#define __KIT_SESSION_H__

#include "Ref"
#include "List.h"

class Session : public Ref
{
public:
    static uint32_t session_id_count_;
    KIT_CREATE_FUNC(Session)
    Session();
    virtual ~Session();

private:
    uint32_t sid_;
    List<Buffer*> packet_list_;
};

#endif


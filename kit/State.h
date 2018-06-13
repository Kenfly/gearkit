#ifndef __KIT_STATE_H__
#define __KIT_STATE_H__


#include "Ref.h"
#include "Map.h"

#ifndef KIT_STATE_LENGTH
#define KIT_STATE_LENGTH 16
#endif

namespace kit {

class StateMgr;

class State : public Ref
{
friend StateMgr;
public:
    virtual void on(){};
    virtual void off(){};
    virtual void update(){};
protected:
    StateMgr* mgr_;
};

class StateMgr : public Ref
{
public:
    StateMgr();
    virtual ~StateMgr();

    bool isState(int state_id);
    State* getState();
    void setState(int state_id);

    virtual void registerState(int state_id, State* state);
    virtual void unregisterState(int state_id);

    void update();
protected:
    virtual bool baseInit();

    State* state_;
    int state_id_;

    typedef Map<int, State*, KIT_STATE_LENGTH> StateMap;
    StateMap state_map_;
};

} // namespace kit

#endif


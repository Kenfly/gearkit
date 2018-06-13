#include "State.h"

namespace kit {

/*
 * init state
 */
class StateNone : public State
{
friend StateMgr;
public:
    virtual void off() { release(); };
private:
    StateNone(){};
};


StateMgr::StateMgr()
: state_(nullptr)
, state_id_(-1)
{
}

StateMgr::~StateMgr()
{
}

bool StateMgr::baseInit()
{
    state_ = new StateNone();
    return true;
}

State* StateMgr::getState()
{
    return state_;
}

bool StateMgr::isState(int state_id)
{
    return state_id_ == state_id;
}

void StateMgr::setState(int state_id)
{
    State* state;
    if (!state_map_.get(state_id, state))
    {
        return;
    }

    state_->off();
    state_->mgr_ = this;
    state->on();
    state_ = state;
}

void StateMgr::registerState(int state_id, State* state)
{
    unregisterState(state_id);
    state_map_.set(state_id, state);
    state->retain();
}

void StateMgr::unregisterState(int state_id)
{
    state_map_.del(state_id, [](State* state){
        state->release();
    });
}

void StateMgr::update()
{
    state_->update();
}

} // namespace kit


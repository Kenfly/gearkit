#ifndef __KIT_CLIENT_STATE_H__
#define __KIT_CLIENT_STATE_H__

#include "State.h"

namespace kit {

class IClient;
class ClientStateBase;

enum class ClientState
{
    None,
    Inited,
    Connecting,
    Working,
    Disconnect,
    Closed,
};

inline constexpr int toint(ClientState s)
{
    return static_cast<int>(s);
}

class ClientStateMgr : public StateMgr
{
public:
    ClientStateMgr(IClient* client);
    IClient* getContext() const { return client_; }
    virtual void setState(ClientState state);
    virtual void registerState(ClientState state_id, ClientStateBase* state);
protected:
    virtual bool baseInit();
private:
    IClient* client_;
};

class ClientStateBase : public State
{
    friend ClientStateMgr;
protected:
    IClient* client_;
};

/*
 * inited
 */
class ClientStateInited : public ClientStateBase
{
public:
    KIT_CREATE_FUNC(ClientStateInited)
};

/*
 * connecting state
 */
class ClientStateConnecting : public ClientStateBase
{
public:
    KIT_CREATE_FUNC(ClientStateConnecting)
    virtual void update();
private:
};

/*
 * working state
 */
class ClientStateWorking : public ClientStateBase
{
public:
    KIT_CREATE_FUNC(ClientStateWorking)
    virtual void update();
};

/*
 * disconnect state
 */
class ClientStateDisconnect : public ClientStateBase
{
public:
    KIT_CREATE_FUNC(ClientStateDisconnect)
};

/*
 * closed state
 */
class ClientStateClosed : public ClientStateBase
{
public:
    KIT_CREATE_FUNC(ClientStateClosed)
};

} // namespace kit

#endif


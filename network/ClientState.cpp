#include "ClientState.h"
#include "Client.h"

namespace kit {

ClientStateMgr::ClientStateMgr(IClient* client)
: client_(client)
{
}

bool ClientStateMgr::baseInit()
{
    if (!StateMgr::baseInit())
        return false;
    registerState(ClientState::Inited, ClientStateInited::create());
    registerState(ClientState::Connecting, ClientStateInited::create());
    registerState(ClientState::Working, ClientStateInited::create());
    registerState(ClientState::Disconnect, ClientStateInited::create());
    registerState(ClientState::Closed, ClientStateClosed::create());
    return true;
}

void ClientStateMgr::setState(ClientState state)
{
    StateMgr::setState(toint(state));
}

void ClientStateMgr::registerState(ClientState state_id, ClientStateBase* state)
{
    state->client_ = client_;
    StateMgr::registerState(toint(state_id), state);
}


/*
 * connecting state
 */

void ClientStateConnecting::update()
{
}

/*
 * working state
 */

void ClientStateWorking::update()
{
}

} // namespace kit

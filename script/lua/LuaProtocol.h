#ifndef __LUA_PROTOCOL_H__
#define __LUA_PROTOCOL_H__

#include <functional>
#include "PacketHandler.h"
#include "LuaIntf/LuaIntf.h"

struct lua_State;

namespace kit {
    class Protocol;
}


typedef std::function<void()> LuaProtocolHandler;

class LuaPacketProtocolHandler : public kit::PacketProtocolHandler
{
public:
    KIT_CREATE_FUNC(LuaPacketProtocolHandler)

    LuaPacketProtocolHandler();
    virtual ~LuaPacketProtocolHandler();

    virtual bool baseInit();

    virtual void onProtocol(int32_t sid, kit::Protocol* pto);
    // 用lua table配置方式注册协议
    void registerProtocol(int32_t pid, const LuaIntf::LuaRef& tb);

    void setHandler(LuaProtocolHandler handler) { handler_ = handler; };

private:
    LuaProtocolHandler handler_;
};

#endif


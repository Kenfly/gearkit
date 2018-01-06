#ifndef __LUA_PROTOCOL_H__
#define __LUA_PROTOCOL_H__

#include <functional>
#include "PacketHandler.h"
#include "lua.hpp"
#include "LuaIntf/LuaIntf.h"

struct lua_State;

namespace kit {
    class PTDataTable;
    class Protocol;
    class PTDataCreator;
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
    void registerProtocol(int32_t pid, lua_State* L);
    void unregisterProtocol(int32_t pid);

    void setHandler(LuaProtocolHandler handler) { handler_ = handler; };
private:
    bool registerTable(kit::PTDataTable* p, const LuaIntf::LuaRef& table);
private:
    LuaProtocolHandler handler_;
    kit::PTDataCreator* creator_;
};

#endif


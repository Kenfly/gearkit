#ifndef __LUA_PROTOCOL_H__
#define __LUA_PROTOCOL_H__

#include "Protocol.h"

struct lua_State;
class LuaPTCreator;

namespace LuaIntf {
    class LuaRef;
}

class LuaProtocol : public kit::Protocol
{
public:
    KIT_CREATE_FUNC(LuaProtocol)
    LuaProtocol();
    virtual ~LuaProtocol();

    virtual bool baseInit();
    // 用lua table配置方式注册协议
    void buildFromTable(lua_State* L);

    void unserializeFromTable(lua_State* L);
    int32_t serializeToTable(lua_State* L);
private:
    bool registerTable(kit::PTTable* p, const LuaIntf::LuaRef* table);

    LuaPTCreator* creator_ = nullptr;
};

class LuaPTCreator : public kit::PTDataCreator
{
public:
    KIT_CREATE_FUNC(LuaPTCreator)
    LuaPTCreator();
    virtual ~LuaPTCreator();
    void setDataValue(kit::PTData* data, const LuaIntf::LuaRef* t);
private:
    template<typename T>
    void unsValue(kit::PTData* data, const LuaIntf::LuaRef* t);
    void unsArray(kit::PTData* data, const LuaIntf::LuaRef* table);
    void unsTable(kit::PTData* data, const LuaIntf::LuaRef* table);

    typedef void (LuaPTCreator::* PTValueAPI)(kit::PTData*, const LuaIntf::LuaRef*);
    PTValueAPI value_apis_[toint(kit::PTType::MAX)];
};

#endif


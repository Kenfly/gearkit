#include "LuaProtocol.h"
#include "Protocol.h"
#include "lua.hpp"
#include "Logger.h"


class PtoHandler : public kit::ProtocolHandler
{
public:
    PtoHandler(LuaPacketProtocolHandler* parent)
    : parent_(parent)
    {
    }
    virtual void onProtocol(int32_t sid, kit::Protocol* pto)
    {
        parent_->onProtocol(sid, pto);
    }
private:
    LuaPacketProtocolHandler* parent_;

};

// lua protocol
LuaPacketProtocolHandler::LuaPacketProtocolHandler()
: handler_(nullptr)
{
}

LuaPacketProtocolHandler::~LuaPacketProtocolHandler()
{
}

// init protocol handler
bool LuaPacketProtocolHandler::baseInit()
{
    if (!kit::PacketProtocolHandler::baseInit())
        return false;

    PtoHandler* handler = new PtoHandler(this);
    setProtocolHandler(handler);
    handler->release();

    return true;
}

/*
 * 一项配置用 {key, value_type}
 * value_type = [bBwWiIlLvfds t a]
 * t--table
 * a--array
 * 如果value_type为a，则配置格式为{key,a,value_type}
 * register a protocol by table
 *  {
 *      {"key1", "[bBwWiIlLvfds]/t/a"},
 *      {"key2", t},
 *      {
 *          {"key1", "i"},
 *          {"key2", "s"}
 *      }
 *      {"key3", a, "[bBwWiIlLvfds]/t/a"},
 *
 *  }
 */
void LuaPacketProtocolHandler::registerProtocol(int32_t pid, const LuaIntf::LuaRef& table)
{
    kit::Protocol* pto = kit::Protocol::create();
    pto->init(pid);

    if (!table.isTable())
    {
        ERR("[LuaPacketProtocolHandler](registerProtocol) param not a table!");
        return;
    }
    //for (auto& e : table)
    {
    }

    kit::PacketProtocolHandler::registerProtocol(pid, pto);
}

void LuaPacketProtocolHandler::onProtocol(int32_t sid, kit::Protocol* pto)
{
}


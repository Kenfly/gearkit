#include "LuaProtocol.h"
#include <stack>
#include "Protocol.h"
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
, creator_(NULL)
{
}

LuaPacketProtocolHandler::~LuaPacketProtocolHandler()
{
    KIT_SAFE_RELEASE(creator_);
}

// init protocol handler
bool LuaPacketProtocolHandler::baseInit()
{
    if (!kit::PacketProtocolHandler::baseInit())
        return false;

    PtoHandler* handler = new PtoHandler(this);
    setProtocolHandler(handler);
    handler->release();


    KIT_SAFE_RELEASE(creator_);
    creator_ = kit::PTDataCreator::create(false);
    return true;
}

/*
 * 一项配置用 {key, value_type}
 * 如果value_type是PT_TYPE_[U]INT32/PT_TYPE_[U]INT64则有第三个参数bool,表示是否fixed,默认是可变长varint
 * table : {key, PT_TYPE_TABLE, table}
 * array : {key, PT_TYPE_ARRAY, table or PT_TYPE_XXX}
 * 如果value_type为a，则配置格式为{key,a,value_type}
 * register a protocol by table
 *  {
 *      {"key1", TYPE_INT32, true},
 *      {"key2", TYPE_TABLE, 
 *          {"key2_1", TYPE_STRING},
 *          {"key2_2", TYPE_ARRAY, TYPE_INT32}
 *      },
 *      {"key3", TYPE_INT32},
 *  }
 */

bool LuaPacketProtocolHandler::registerTable(kit::PTDataTable* p, const LuaIntf::LuaRef& table)
{
    for (auto& e : table)
    {
        auto v = e.value<LuaIntf::LuaRef>();
        if (!v.isTable())
        {
            ERR("[LuaPacketProtocolHandler](registerTable) desc not table! key:%s", e.key<const std::string&>().c_str());
            return false;
        }
        int len = v.len();
        if (len < 2)
        {
            ERR("[LuaPacketProtocolHandler](registerTable) not table! key:%s", e.key<const std::string&>().c_str());
            return false;
        }
        std::string key = v.get<std::string>(1);
        int32_t value_type = v.get<int32_t>(2);
        kit::PTData* info = creator_->createPTData(value_type);
        if (info == NULL)
        {
            ERR("[LuaPacketProtocolHandler](registerTable) error type! key:%s, type:%d", key.c_str(), value_type);
            return false;
        }
        p->addData(info);

        if (value_type == PT_TYPE_TABLE)
        {
            auto sub_table = v.get<LuaIntf::LuaRef>(3);
            if (!sub_table.isTable())
            {
                ERR("[LuaPacketProtocolHandler](registerTable) not table! key:%s", key.c_str());
                return false;
            }
            if (!registerTable(static_cast<kit::PTDataTable*>(info), sub_table))
            {
                return false;
            }
        }
        else if (value_type == PT_TYPE_ARRAY)
        {
            if (len < 3)
            {
                ERR("[LuaPacketProtocolHandler](registerTable) array need 3 param! key:%s", key.c_str());
                return false;
            }
            auto item_v = v.get<LuaIntf::LuaRef>(3);
            if (item_v.isTable())
            {
                auto item_template = creator_->createPTData(PT_TYPE_TABLE);
                if (!registerTable(static_cast<kit::PTDataTable*>(item_template), item_v))
                    return false;
                (static_cast<kit::PTDataArray*>(info))->setTemplate(item_template);
            }
            else
            {
                int32_t item_type = item_v.toValue<int32_t>();
                auto item_template = creator_->createPTData(item_type);
                if (item_template == NULL)
                {
                    ERR("[LuaPacketProtocolHandler](registerTable) erryr type! key:%s", key.c_str());
                    return false;
                }
                (static_cast<kit::PTDataArray*>(info))->setTemplate(item_template);
            }
        }
        else
        {
            if (len >= 3)
            {
                info->setFixed(v.get<bool>(3));
            }
        }
    }
    return true;
}

void LuaPacketProtocolHandler::registerProtocol(int32_t pid, lua_State* L)
{
    LuaIntf::LuaRef table = LuaIntf::Lua::pop<LuaIntf::LuaRef>(L);
    if (!table.isTable())
    {
        ERR("[LuaPacketProtocolHandler](registerProtocol) param not a table! pid:%d", pid);
        return;
    }

    kit::Protocol* pto = kit::Protocol::create();
    pto->init(pid);

    if (!registerTable(pto, table))
    {
        ERR("[LuaPacketProtocolHandler](registerProtocol) register error! pid:%d", pid);
        return;
    }

    kit::PacketProtocolHandler::registerProtocol(pid, pto);
}

void LuaPacketProtocolHandler::unregisterProtocol(int32_t pid)
{
    kit::PacketProtocolHandler::unregisterProtocol(pid);
}

void LuaPacketProtocolHandler::onProtocol(int32_t sid, kit::Protocol* pto)
{
}


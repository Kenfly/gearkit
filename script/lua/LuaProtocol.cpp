#include "LuaProtocol.h"
#include <stack>
#include "lua.hpp"
#include "LuaIntf/LuaIntf.h"
#include "Logger.h"

using pt = kit::PTValueType;

/*
 * 一项配置用 
 * {key, value_type}
 * table : {key, TABLE, table}
 * array : {key, ARRAY, table or value_type}
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

LuaProtocol::LuaProtocol()
{
}

LuaProtocol::~LuaProtocol()
{
    KIT_SAFE_RELEASE(creator_);
}

bool LuaProtocol::baseInit()
{
    KIT_SAFE_RELEASE(creator_);
    creator_ = LuaPTCreator::create(false);
    return kit::Protocol::baseInit();
}

void LuaProtocol::buildFromTable(lua_State* L)
{
    LuaIntf::LuaRef table = LuaIntf::Lua::pop<LuaIntf::LuaRef>(L);
    if (!table.isTable())
    {
        ERR("[LuaProtocol](buildFromTable) param not a table! pid:%d", pid_);
        return;
    }

    if (!registerTable(this, &table))
    {
        ERR("[LuaProtocol](buildFromTable) register error! pid:%d", pid_);
        return;
    }
}

bool LuaProtocol::registerTable(kit::PTTable* p, const LuaIntf::LuaRef* table)
{
    for (auto& e : *table)
    {
        auto v = e.value<LuaIntf::LuaRef>();
        if (!v.isTable())
        {
            ERR("[LuaProtocol](registerTable) desc not table! key:%s", e.key<const std::string&>().c_str());
            return false;
        }
        int len = v.len();
        if (len < 2)
        {
            ERR("[LuaProtocol](registerTable) not table! key:%s", e.key<const std::string&>().c_str());
            return false;
        }
        std::string key = v.get<std::string>(1);
        kit::PTValueType value_type = static_cast<kit::PTValueType>(v.get<uint8_t>(2));
        kit::PTData* info = creator_->createPTData(value_type);
        if (info == NULL)
        {
            ERR("[LuaProtocol](registerTable) error type! key:%s, type:%d", key.c_str(), value_type);
            return false;
        }
        info->setKey(key);
        p->addData(info);

        if (value_type == kit::PTValueType::TABLE)
        {
            auto sub_table = v.get<LuaIntf::LuaRef>(3);
            if (!sub_table.isTable())
            {
                ERR("[LuaProtocol](registerTable) not table! key:%s", key.c_str());
                return false;
            }
            if (!registerTable(reinterpret_cast<kit::PTTable*>(info), &sub_table))
            {
                return false;
            }
        }
        else if (value_type == kit::PTValueType::ARRAY)
        {
            if (len < 3)
            {
                ERR("[LuaProtocol](registerTable) array need 3 param! key:%s", key.c_str());
                return false;
            }
            auto item_v = v.get<LuaIntf::LuaRef>(3);
            if (item_v.isTable())
            {
                auto item_template = creator_->createPTData(kit::PTValueType::ARRAY);
                if (!registerTable(reinterpret_cast<kit::PTTable*>(item_template), &item_v))
                    return false;
                (reinterpret_cast<kit::PTArray*>(info))->setTemplate(item_template);
            }
            else
            {
                kit::PTValueType item_type = static_cast<kit::PTValueType>(item_v.toValue<uint8_t>());
                auto item_template = creator_->createPTData(item_type);
                if (item_template == NULL)
                {
                    ERR("[LuaProtocol](registerTable) erryr type! key:%s", key.c_str());
                    return false;
                }
                (reinterpret_cast<kit::PTArray*>(info))->setTemplate(item_template);
            }
        }
        else
        {
        }
    }
    return true;
}

void LuaProtocol::unserializeFromTable(lua_State* L)
{
    LuaIntf::LuaRef table = LuaIntf::Lua::pop<LuaIntf::LuaRef>(L);
    if (!table.isTable())
    {
        ERR("[LuaProtocol](serializeFromTable) param not a table! pid:%d", pid_);
        return;
    }
    if (!unserializeTable(this, &table))
    {
        ERR("[LuaProtocol](unserializeFromTable) unserialize error! pid:%d", pid_);
        return;
    }
}

bool LuaProtocol::unserializeTable(kit::PTTable* p, const LuaIntf::LuaRef* table)
{
    for (DataVec::iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        creator_->setDataValue(*ix, table);
    }
    return true;
}

int32_t LuaProtocol::serializeToTable(lua_State* L)
{
    return 1;
}


// creator
LuaPTCreator::LuaPTCreator()
{
    value_apis_[toint(pt::INT8)] = &LuaPTCreator::unsValue<int8_t>;
    value_apis_[toint(pt::UINT8)] = &LuaPTCreator::unsValue<uint8_t>;
    value_apis_[toint(pt::INT16)] = &LuaPTCreator::unsValue<int16_t>;
    value_apis_[toint(pt::UINT16)] = &LuaPTCreator::unsValue<uint16_t>;
    value_apis_[toint(pt::INT32)] = &LuaPTCreator::unsValue<int32_t>;
    value_apis_[toint(pt::UINT32)] = &LuaPTCreator::unsValue<uint32_t>;
    value_apis_[toint(pt::INT64)] = &LuaPTCreator::unsValue<int64_t>;
    value_apis_[toint(pt::UINT64)] = &LuaPTCreator::unsValue<uint64_t>;
    value_apis_[toint(pt::VARINT)] = &LuaPTCreator::unsValue<uint32_t>;
    value_apis_[toint(pt::STRING)] = &LuaPTCreator::unsValue<std::string>;

    value_apis_[toint(pt::ARRAY)] = &LuaPTCreator::unsArray;
    value_apis_[toint(pt::TABLE)] = &LuaPTCreator::unsTable;
}

LuaPTCreator::~LuaPTCreator()
{
}

void LuaPTCreator::setDataValue(kit::PTData* data, const LuaIntf::LuaRef* table)
{
    pt type = data->getValueType();
    DCHECK(type != pt::ARRAY && type != pt::TABLE, 
        "[LuaPTCreator](setDataValue) value type invalid!");
    PTValueAPI func = value_apis_[toint(type)];
    ((this->*(func))(data, table));
}

template<typename T>
void LuaPTCreator::unsValue(kit::PTData* data, const LuaIntf::LuaRef* table)
{
    kit::PTValue<T>* p = reinterpret_cast<kit::PTValue<T>*>(data);
    const std::string& key = data->getKey();
    if (table->has(key))
    {
        p->setValue(table->get<T>(key));
    } else {
        p->resetValue();
    }
}

void LuaPTCreator::unsArray(kit::PTData* data, const LuaIntf::LuaRef* table)
{
}

void LuaPTCreator::unsTable(kit::PTData* data, const LuaIntf::LuaRef* table)
{
}


#include "LuaProtocol.h"
#include <stack>
#include "lua.hpp"
#include "LuaIntf/LuaIntf.h"
#include "Logger.h"
#include "Functions.h"

using pt = kit::PTType;

/*
 * 一项配置用 
 * {key, type}
 * table : {key, TABLE, table}
 * array : {key, ARRAY, table or type}
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
        kit::PTType type = static_cast<kit::PTType>(v.get<uint8_t>(2));
        kit::PTData* info = creator_->createPTData(type);
        if (info == NULL)
        {
            ERR("[LuaProtocol](registerTable) error type! key:%s, type:%d", key.c_str(), type);
            return false;
        }
        info->setKey(key);
        p->addData(info);

        if (type == kit::PTType::TABLE)
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
        else if (type == kit::PTType::ARRAY)
        {
            if (len < 3)
            {
                ERR("[LuaProtocol](registerTable) array need 3 param! key:%s", key.c_str());
                return false;
            }
            auto item_v = v.get<LuaIntf::LuaRef>(3);
            if (item_v.isTable())
            {
                auto item_template = creator_->createPTData(kit::PTType::ARRAY);
                if (!registerTable(reinterpret_cast<kit::PTTable*>(item_template), &item_v))
                    return false;
                (reinterpret_cast<kit::PTArray*>(info))->setTemplate(item_template);
            }
            else
            {
                kit::PTType item_type = static_cast<kit::PTType>(item_v.toValue<uint8_t>());
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
    creator_->setDataValue(this, &table);
}

int32_t LuaProtocol::serializeToTable(lua_State* L)
{
    return 1;
}


// creator
LuaPTCreator::LuaPTCreator()
{
    value_apis_[kit::toint(pt::INT8)] = &LuaPTCreator::unsValue<int8_t>;
    value_apis_[kit::toint(pt::UINT8)] = &LuaPTCreator::unsValue<uint8_t>;
    value_apis_[kit::toint(pt::INT16)] = &LuaPTCreator::unsValue<int16_t>;
    value_apis_[kit::toint(pt::UINT16)] = &LuaPTCreator::unsValue<uint16_t>;
    value_apis_[kit::toint(pt::INT32)] = &LuaPTCreator::unsValue<int32_t>;
    value_apis_[kit::toint(pt::UINT32)] = &LuaPTCreator::unsValue<uint32_t>;
    value_apis_[kit::toint(pt::INT64)] = &LuaPTCreator::unsValue<int64_t>;
    value_apis_[kit::toint(pt::UINT64)] = &LuaPTCreator::unsValue<uint64_t>;
    value_apis_[kit::toint(pt::VARINT)] = &LuaPTCreator::unsValue<uint32_t>;
    value_apis_[kit::toint(pt::STRING)] = &LuaPTCreator::unsValue<std::string>;

    value_apis_[kit::toint(pt::ARRAY)] = &LuaPTCreator::unsArray;
    value_apis_[kit::toint(pt::TABLE)] = &LuaPTCreator::unsTable;
}

LuaPTCreator::~LuaPTCreator()
{
}

void LuaPTCreator::setDataValue(kit::PTData* data, const LuaIntf::LuaRef* t)
{
    pt type = data->getType();
    DCHECK(type != pt::NONE && type != pt::MAX,
        kit::formatString("[LuaPTCreator](setDataValue) invalid type: %d", kit::toint(type)).c_str());
    PTValueAPI func = value_apis_[kit::toint(type)];
    ((this->*(func))(data, t));
}

template<typename T>
void LuaPTCreator::unsValue(kit::PTData* data, const LuaIntf::LuaRef* t)
{
    kit::PTValue<T>* p = reinterpret_cast<kit::PTValue<T>*>(data);
    p->setValue(t->toValue<T>());
}

void LuaPTCreator::unsArray(kit::PTData* data, const LuaIntf::LuaRef* table)
{
    kit::PTArray* array = reinterpret_cast<kit::PTArray*>(data);
    array->clear();
    int len = table->len();
    for (int i = 1; i <= len; ++i)
    {
        auto ref = table->get(i);
        auto item = array->cloneItem();
        setDataValue(item, &ref);
        array->addData(item);
    }
}

void LuaPTCreator::unsTable(kit::PTData* data, const LuaIntf::LuaRef* table)
{
    kit::PTTable* t = reinterpret_cast<kit::PTTable*>(data);
    for (auto p : t->getDatas())
    {
        const std::string& key = p->getKey();
        if (table->has(key))
        {
            LuaIntf::LuaRef subtable = table->get(key);
            setDataValue(p, &subtable);
        } else {
            p->resetValue();
        }
    }
}


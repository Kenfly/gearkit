#include "Protocol.h"

namespace kit {

// pt data array
PTDataArray::PTDataArray()
: item_template(NULL)
{
}

PTDataArray::~PTDataArray()
{
    clear();
    KIT_SAFE_RELEASE(item_template);
}

bool PTDataArray::serialize(Buffer* buffer)
{
    uint32_t len = static_cast<uint32_t>(datas.size());
    if (!buffer->writeVar_uint32_t(len))
        return false;

    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        if (!(*ix)->serialize(buffer))
            return false;
    }

    return true;
}

bool PTDataArray::unserialize(Buffer* buffer)
{
    if (item_template == NULL)
        return false;
    uint32_t len;
    if (!buffer->readVar_uint32_t(&len))
        return false;
    clear();
    datas.resize(len);
    PTData* data;
    for (uint32_t i = 0; i != len; ++i)
    {
        data = item_template->clone();
        if (!data || !data->unserialize(buffer))
            return false;
        addData(data);
    }

    return true;
}

void PTDataArray::addData(PTData* data)
{
    KIT_SAFE_RETAIN(data);
    datas.push_back(data);
}

PTData* PTDataArray::clone() const
{
    PTDataArray* pt = PTDataArray::create();
    if (this->item_template)
    {
        PTData* data = this->item_template->clone();
        pt->setTemplate(data);
    }
    return pt;
}

void PTDataArray::setTemplate(PTData* data)
{
    if (item_template == data)
        return;
    KIT_SAFE_RELEASE(item_template);
    item_template = data;
    KIT_SAFE_RETAIN(item_template);
}

void PTDataArray::clear()
{
    PTData* p;
    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        p = (*ix);
        if (p)
            p->release();
    }
    datas.clear();
}

// pt data table
PTDataTable::~PTDataTable()
{
    clear();
}

bool PTDataTable::serialize(Buffer* buffer)
{
    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        if (!(*ix)->serialize(buffer))
            return false;
    }
    return true;
}

PTData* PTDataTable::clone() const
{
    return NULL;
}

bool PTDataTable::unserialize(Buffer* buffer)
{
    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        if (!(*ix)->unserialize(buffer))
            return false;
    }
    return true;
}

void PTDataTable::addData(PTData* data)
{
    KIT_SAFE_RETAIN(data);
    datas.push_back(data);
}

void PTDataTable::delData(PTData* data)
{
    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        if (*ix == data)
        {
            (*ix)->release();
            datas.erase(ix);
            break;
        }
    }
}

void PTDataTable::clear()
{
    PTData* p;
    for(DataVec::iterator ix = datas.begin(); ix != datas.end(); ++ix)
    {
        p = (*ix);
        if (p)
            p->release();
    }
    datas.clear();
}


// protocol
Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

void Protocol::init(int32_t _pid)
{
    pid = _pid;
}


// PTDataCreator
PTDataCreator::PTDataCreator()
{
    api_table_[PT_TYPE_INT8] = &PTDataCreator::createPTData_int8_t;
    api_table_[PT_TYPE_UINT8] = &PTDataCreator::createPTData_uint8_t;
    api_table_[PT_TYPE_INT16] = &PTDataCreator::createPTData_int16_t;
    api_table_[PT_TYPE_UINT16] = &PTDataCreator::createPTData_uint16_t;
    api_table_[PT_TYPE_INT32] = &PTDataCreator::createPTData_int32_t;
    api_table_[PT_TYPE_UINT32] = &PTDataCreator::createPTData_uint32_t;
    api_table_[PT_TYPE_INT64] = &PTDataCreator::createPTData_int64_t;
    api_table_[PT_TYPE_UINT64] = &PTDataCreator::createPTData_uint64_t;
    api_table_[PT_TYPE_STRING] = &PTDataCreator::createPTData_string;

    api_table_[PT_TYPE_ARRAY] = &PTDataCreator::createPTData_array;
    api_table_[PT_TYPE_TABLE] = &PTDataCreator::createPTData_table;
}

PTDataCreator::~PTDataCreator()
{
}

PTData* PTDataCreator::createPTData(int32_t value_type)
{
    if (value_type < 0 || value_type >= PT_TYPE_MAX)
        return NULL;
    PTDataAPI func = api_table_[value_type];
    return ((this->*(func))());
}

PTData* PTDataCreator::createPTData_int8_t()
{
    return PTDataNormal<int8_t>::create();
}

PTData* PTDataCreator::createPTData_uint8_t()
{
    return PTDataNormal<uint8_t>::create();
}

PTData* PTDataCreator::createPTData_int16_t()
{
    return PTDataNormal<int16_t>::create();
}

PTData* PTDataCreator::createPTData_uint16_t()
{
    return PTDataNormal<uint16_t>::create();
}

PTData* PTDataCreator::createPTData_int32_t()
{
    return PTDataNormal<int32_t>::create();
}

PTData* PTDataCreator::createPTData_uint32_t()
{
    return PTDataNormal<uint32_t>::create();
}

PTData* PTDataCreator::createPTData_int64_t()
{
    return PTDataNormal<int64_t>::create();
}

PTData* PTDataCreator::createPTData_uint64_t()
{
    return PTDataNormal<uint64_t>::create();
}

PTData* PTDataCreator::createPTData_string()
{
    return PTDataNormal<std::string>::create();
}

PTData* PTDataCreator::createPTData_array()
{
    return PTDataArray::create();
}

PTData* PTDataCreator::createPTData_table()
{
    return PTDataTable::create();
}

} // namespace kit


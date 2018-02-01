#include "Protocol.h"

namespace kit {

// value
template<typename T>
bool PTValue<T>::serialize(Buffer* buffer) const
{
    return (*buffer) << value_;
}

template<typename T>
bool PTValue<T>::unserialize(Buffer* buffer)
{
    return (*buffer) >> value_;
}

template<typename T>
PTData* PTValue<T>::clone() const
{
    return PTValue<T>::create();
}

template<typename T>
std::string PTValue<T>::toString() const
{
    return std::string("{") + key_ + ":" + std::to_string(value_) + "}";
}

template<>
std::string PTValue<std::string>::toString() const
{
    return std::string("{") + key_ + ":" + value_ + "}";
}

// varint
bool PTVarint::serialize(Buffer* buffer) const
{
    return buffer->writeVaruint(value_);
}

bool PTVarint::unserialize(Buffer* buffer)
{
    return buffer->readVaruint(value_);
}

PTData* PTVarint::clone() const
{
    return PTVarint::create();
}

// pt data array
PTArray::PTArray()
: item_template(NULL)
{
}

PTArray::~PTArray()
{
    clear();
    KIT_SAFE_RELEASE(item_template);
}

bool PTArray::serialize(Buffer* buffer) const
{
    size_t len = datas_.size();
    if (!buffer->writeVaruint<size_t>(len))
        return false;

    for(DataVec::const_iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        if (!(*ix)->serialize(buffer))
            return false;
    }

    return true;
}

bool PTArray::unserialize(Buffer* buffer)
{
    if (item_template == NULL)
        return false;
    size_t len;
    if (!buffer->readVaruint<size_t>(len))
        return false;
    clear();
    datas_.resize(len);
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

void PTArray::addData(PTData* data)
{
    KIT_SAFE_RETAIN(data);
    datas_.push_back(data);
}

PTData* PTArray::clone() const
{
    PTArray* pt = PTArray::create();
    if (this->item_template)
    {
        PTData* data = this->item_template->clone();
        pt->setTemplate(data);
    }
    return pt;
}

std::string PTArray::toString() const
{
    std::string s = "{" + key_ + ":[";
    for (DataVec::const_iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        s += (*ix)->toString();
    }
    s += "]}";
    return s;
}

void PTArray::setTemplate(PTData* data)
{
    if (item_template == data)
        return;
    KIT_SAFE_RELEASE(item_template);
    item_template = data;
    KIT_SAFE_RETAIN(item_template);
}

void PTArray::clear()
{
    PTData* p;
    for(DataVec::iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        p = (*ix);
        if (p)
            p->release();
    }
    datas_.clear();
}

// pt data table
PTTable::~PTTable()
{
    clear();
}

bool PTTable::serialize(Buffer* buffer) const
{
    for(DataVec::const_iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        if (!(*ix)->serialize(buffer))
            return false;
    }
    return true;
}

PTData* PTTable::clone() const
{
    return NULL;
}

std::string PTTable::toString() const
{
    std::string s = "{" + key_ + ":\n";
    for (DataVec::const_iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        s += (*ix)->toString() + "\n";
    }
    s += "}";
    return s;
}

bool PTTable::unserialize(Buffer* buffer)
{
    for(DataVec::iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        if (!(*ix)->unserialize(buffer))
            return false;
    }
    return true;
}

void PTTable::addData(PTData* data)
{
    KIT_SAFE_RETAIN(data);
    datas_.push_back(data);
}

void PTTable::delData(PTData* data)
{
    for(DataVec::iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        if (*ix == data)
        {
            (*ix)->release();
            datas_.erase(ix);
            break;
        }
    }
}

void PTTable::clear()
{
    PTData* p;
    for(DataVec::iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        p = (*ix);
        if (p)
            p->release();
    }
    datas_.clear();
}


// protocol
Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

void Protocol::init(int32_t pid)
{
    pid_ = pid;
}

std::string Protocol::toString() const
{
    std::string s = "[" + std::to_string(pid_) + "]\n";
    s += PTTable::toString();
    return s;
}

// PTDataCreator
PTDataCreator::PTDataCreator()
{
    using pt = PTValueType ;
    api_table_[toint(pt::INT8)] = &PTDataCreator::createValue<int8_t>;
    api_table_[toint(pt::UINT8)] = &PTDataCreator::createValue<uint8_t>;
    api_table_[toint(pt::INT16)] = &PTDataCreator::createValue<int16_t>;
    api_table_[toint(pt::UINT16)] = &PTDataCreator::createValue<uint16_t>;
    api_table_[toint(pt::INT32)] = &PTDataCreator::createValue<int32_t>;
    api_table_[toint(pt::UINT32)] = &PTDataCreator::createValue<uint32_t>;
    api_table_[toint(pt::INT64)] = &PTDataCreator::createValue<int64_t>;
    api_table_[toint(pt::UINT64)] = &PTDataCreator::createValue<uint64_t>;
    api_table_[toint(pt::VARINT)] = &PTDataCreator::createVarint;
    api_table_[toint(pt::STRING)] = &PTDataCreator::createValue<std::string>;

    api_table_[toint(pt::ARRAY)] = &PTDataCreator::createArray;
    api_table_[toint(pt::TABLE)] = &PTDataCreator::createTable;
}

PTDataCreator::~PTDataCreator()
{
}

PTData* PTDataCreator::createPTData(PTValueType value_type)
{
    DCHECK(value_type != PTValueType::NONE && value_type != PTValueType::MAX,
        "[PTDataCreator](createPTData) value_type invalid!");
    PTDataAPI func = api_table_[toint(value_type)];
    PTData* data = ((this->*(func))());
    data->setValueType(value_type);
    return data;
}

template<typename T>
PTData* PTDataCreator::createValue()
{
    return PTValue<T>::create();
}


PTData* PTDataCreator::createVarint()
{
    return PTVarint::create();
}

PTData* PTDataCreator::createArray()
{
    return PTArray::create();
}

PTData* PTDataCreator::createTable()
{
    return PTTable::create();
}

} // namespace kit


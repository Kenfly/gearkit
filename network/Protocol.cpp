#include "Protocol.h"
#include "Buffer.h"

namespace kit {

// value
template<typename T>
bool PTValue<T>::serialize(Buffer* buffer) const
{
    const T& v = value_;
    return (*buffer) << v;
}

template<typename T>
bool PTValue<T>::unserialize(Buffer* buffer)
{
    return (*buffer) >> value_;
}

template<typename T>
PTData* PTValue<T>::clone() const
{
    PTValue<T>* new_data = PTValue<T>::create();
    new_data->type_ = this->type_;
    return new_data;
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
: item_template_(NULL)
{
}

PTArray::~PTArray()
{
    clear();
    KIT_SAFE_RELEASE(item_template_);
}

bool PTArray::serialize(Buffer* buffer) const
{
    size_t len = datas_.size();
    if (!buffer->writeVaruint<size_t>(len))
        return false;

    for (auto p : datas_)
    {
        if (!p->serialize(buffer))
            return false;
    }

    return true;
}

bool PTArray::unserialize(Buffer* buffer)
{
    if (item_template_ == NULL)
        return false;
    size_t len;
    if (!buffer->readVaruint<size_t>(len))
        return false;
    clear();
    datas_.resize(len);
    PTData* data;
    for (uint32_t i = 0; i != len; ++i)
    {
        data = item_template_->clone();
        if (!data || !data->unserialize(buffer))
            return false;
        addData(data);
    }

    return true;
}

void PTArray::resetValue()
{
    clear();
}

void PTArray::addData(PTData* data)
{
    KIT_SAFE_RETAIN(data);
    datas_.push_back(data);
}

PTData* PTArray::clone() const
{
    PTArray* pt = PTArray::create();
    if (this->item_template_)
    {
        PTData* data = this->item_template_->clone();
        pt->setTemplate(data);
    }
    return pt;
}

std::string PTArray::toString() const
{
    std::string s = "{" + key_ + ":[";
    for (auto p : datas_)
    {
        s += p->toString();
    }
    s += "]}";
    return s;
}

void PTArray::setTemplate(PTData* data)
{
    if (item_template_ == data)
        return;
    KIT_SAFE_RELEASE(item_template_);
    item_template_ = data;
    KIT_SAFE_RETAIN(item_template_);
}

PTData* PTArray::cloneItem()
{
    return item_template_->clone();
}

void PTArray::clear()
{
    for(auto p : datas_)
    {
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
    for (auto p : datas_)
    {
        if (!p->serialize(buffer))
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
    for (auto p : datas_)
    {
        s += p->toString() + "\n";
    }
    s += "}";
    return s;
}

bool PTTable::unserialize(Buffer* buffer)
{
    for (auto p : datas_)
    {
        if (!p->unserialize(buffer))
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
    for(PTDataVec::iterator ix = datas_.begin(); ix != datas_.end(); ++ix)
    {
        if (*ix == data)
        {
            (*ix)->release();
            datas_.erase(ix);
            break;
        }
    }
}

void PTTable::resetValue()
{
    for (auto p : datas_)
    {
        p->resetValue();
    }
}

void PTTable::clear()
{
    for (auto p : datas_)
    {
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

void Protocol::init(ProtocolID pid)
{
    pid_ = pid;
}

std::string Protocol::toString() const
{
    std::string s = "["+std::to_string(pid_)+"]\n";
    s += PTTable::toString();
    return s;
}

// PTDataCreator
PTDataCreator::PTDataCreator()
{
    using pt = PTType ;
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

PTData* PTDataCreator::createPTData(PTType type)
{
    DCHECK(type != PTType::NONE && type != PTType::MAX,
        "[PTDataCreator](createPTData) type invalid!");
    PTDataAPI func = api_table_[toint(type)];
    PTData* data = ((this->*(func))());
    data->setType(type);
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


#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include <string>
#include <functional>

#define PT_TYPE_INT8   1
#define PT_TYPE_UINT8  2
#define PT_TYPE_INT16  3
#define PT_TYPE_UINT16 4
#define PT_TYPE_INT32  5
#define PT_TYPE_UINT32 6
#define PT_TYPE_INT64  7
#define PT_TYPE_UINT64 8
#define PT_TYPE_STRING 9

#define PT_TYPE_ARRAY  10
#define PT_TYPE_TABLE  11

#define PT_TYPE_MAX 12

namespace kit {

// data

enum PTDataType
{
    PTDATA_TYPE_NONE = 0,
    PTDATA_TYPE_NORMAL = 1,
    PTDATA_TYPE_ARRAY = 2,
    PTDATA_TYPE_TABLE = 3,
    PTDATA_TYPE_PROTOCOL = 4,
};

class PTData : public Ref
{
public:
    virtual ~PTData() {}

    virtual PTDataType getType() const { return PTDATA_TYPE_NONE; }

    virtual bool serialize(Buffer* buffer) = 0;
    virtual bool unserialize(Buffer* buffer) = 0;
    virtual PTData* clone() const { return NULL; }
    virtual void setFixed(bool v) {}
};

// normal value
template<typename ValueType>
class PTDataNormal : public PTData
{
public:
    KIT_CREATE_FUNC(PTDataNormal<ValueType>)
    PTDataNormal() : fixed(false) { }

    virtual PTDataType getType() const { return PTDATA_TYPE_NORMAL; }
    virtual const std::string& getKey() const { return key; }

    virtual bool serialize(Buffer* buffer)
    {
        return (*buffer) << value;
    }
    virtual bool unserialize(Buffer* buffer)
    {
        return (*buffer) >> value;
    }

    virtual PTData* clone() const
    {
        return PTDataNormal<ValueType>::create();
    }

    virtual void setFixed(bool v) { this->fixed = v; }

    virtual const ValueType& getValue() const { return value; }

    std::string key;
    ValueType value;
    bool fixed;
};


// array
class PTDataArray : public PTData
{
public:
    KIT_CREATE_FUNC(PTDataArray)

    PTDataArray();
    virtual ~PTDataArray();

    virtual PTDataType getType() const { return PTDATA_TYPE_ARRAY; }
    virtual bool serialize(Buffer* buffer);
    virtual bool unserialize(Buffer* Buffer);
    virtual PTData* clone() const;

    void addData(PTData* data);
    // 设置模版
    void setTemplate(PTData* data);
    void clear();

    typedef std::vector<PTData*> DataVec;
    DataVec datas;
    PTData* item_template;
};

// Table
class PTDataTable : public PTData
{
public:
    KIT_CREATE_FUNC(PTDataTable)

    virtual ~PTDataTable();

    virtual PTDataType getType() const { return PTDATA_TYPE_TABLE; }

    virtual bool serialize(Buffer* buffer);
    virtual bool unserialize(Buffer* buffer);
    virtual PTData* clone() const;

    void addData(PTData* data);
    void delData(PTData* data);
    virtual void clear();

    typedef std::vector<PTData*> DataVec;
    DataVec datas;
};

// protocol
class Protocol : public PTDataTable
{
public:
    KIT_CREATE_FUNC(Protocol)
    Protocol();
    virtual ~Protocol();

    virtual PTDataType getType() const { return PTDATA_TYPE_PROTOCOL; }

    virtual void init(int32_t _pid);
public:
    int32_t pid;
};

class PTDataCreator : public Ref
{
public:
    KIT_CREATE_FUNC(PTDataCreator)

    PTDataCreator();
    virtual ~PTDataCreator();

    PTData* createPTData(int32_t value_type);
private:
    PTData* createPTData_int8_t();
    PTData* createPTData_uint8_t();
    PTData* createPTData_int16_t();
    PTData* createPTData_uint16_t();
    PTData* createPTData_int32_t();
    PTData* createPTData_uint32_t();
    PTData* createPTData_int64_t();
    PTData* createPTData_uint64_t();
    PTData* createPTData_string();

    PTData* createPTData_array();
    PTData* createPTData_table();

    typedef PTData* (PTDataCreator::* PTDataAPI)();
    PTDataAPI api_table_[PT_TYPE_MAX];
};


} // namespace kit

#endif


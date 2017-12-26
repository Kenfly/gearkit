#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include <string>
#include <functional>

namespace kit {

// data

enum PTDataType
{
    PTDATA_TYPE_NONE = 0,
    PTDATA_TYPE_NORMAL = 1,
    PTDATA_TYPE_TABLE = 2,
    PTDATA_TYPE_ARRAY = 3,
    PTDATA_TYPE_PROTOCOL = 4,
};

class PTData : public Ref
{
public:
    virtual ~PTData() {}

    virtual PTDataType getType() const { return PTDATA_TYPE_NONE; }

    virtual void serialize(Buffer* buffer) = 0;
    virtual void unserialize(Buffer* buffer) = 0;

    virtual const std::string& getKey() const { return key; }

    std::string key;
};

// normal value
template<typename ValueType>
class PTDataNormal : public PTData
{
public:
    KIT_CREATE_FUNC(PTDataNormal<ValueType>)

    virtual PTDataType getType() const { return PTDATA_TYPE_NORMAL; }

    virtual void serialize(Buffer* buffer)
    {
        (*buffer) << value;
    }
    virtual void unserialize(Buffer* buffer)
    {
        (*buffer) >> value;
    }

    virtual const ValueType& getValue() const { return value; }

    ValueType value;
};

// array
template<typename ValueType>
class PTDataArray : public PTData
{
public:
    KIT_CREATE_FUNC(PTDataArray)

    virtual ~PTDataArray();

    virtual PTDataType getType() const { return PTDATA_TYPE_ARRAY; }

    virtual void serialize(Buffer* buffer)
    {
        //(*buffer) << array.
    }

    virtual void unserialize(Buffer* Buffer)
    {
    }

    std::vector<ValueType> array;
};

// Table
class PTDataTable : public PTData
{
public:
    KIT_CREATE_FUNC(PTDataTable)

    virtual ~PTDataTable();

    virtual PTDataType getType() const { return PTDATA_TYPE_TABLE; }

    virtual void serialize(Buffer* buffer);
    virtual void unserialize(Buffer* buffer);

    virtual void addData(PTData* data);
    virtual void delData(PTData* data);
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

} // namespace kit
#endif


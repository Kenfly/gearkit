#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include <string>
#include <functional>

namespace kit {

enum class PTValueType : uint8_t
{
    NONE,
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    INT64,
    UINT64,
    VARINT,
    STRING,

    ARRAY,
    TABLE,
    MAX,
};

enum class PTDataType : uint8_t
{
    NONE,
    VALUE,
    ARRAY,
    TABLE,
    PROTOCOL,
};

inline constexpr int toint(PTValueType t)
{
    return static_cast<int>(t);
}

inline constexpr int toint(PTDataType t)
{
    return static_cast<int>(t);
}

// data
class PTData : public Ref
{
public:
    virtual ~PTData() {}

    virtual PTDataType getType() const { return PTDataType::NONE; }

    virtual bool serialize(Buffer* buffer) const = 0;
    virtual bool unserialize(Buffer* buffer) = 0;

    const std::string& getKey() const { return key_; }
    void setKey(const std::string& k) { key_ = k; }

    PTValueType getValueType() const { return value_type_; }
    void setValueType(PTValueType type) { value_type_ = type; }

    virtual PTData* clone() const { return NULL; }

protected:
    PTValueType value_type_;
    std::string key_;
};

// normal value
template<typename T>
class PTValue : public PTData
{
public:
    KIT_CREATE_FUNC(PTValue<T>)

    virtual PTDataType getType() const { return PTDataType::VALUE; }
    const T& getValue() const { return value_; }
    void setValue(const T& v) { value_ = v; }
    void resetValue() { value_ = 0; }
    void reset();

    virtual bool serialize(Buffer* buffer) const;
    virtual bool unserialize(Buffer* buffer);

    virtual PTData* clone() const;
    virtual std::string toString() const;

protected:
    T value_;
};

template<>
inline void PTValue<std::string>::resetValue()
{
    value_ = "";
}

// varint
class PTVarint : public PTValue<uint32_t>
{
public:
    KIT_CREATE_FUNC(PTVarint);
    virtual bool serialize(Buffer* buffer) const;
    virtual bool unserialize(Buffer* buffer);
    virtual PTData* clone() const;
};

// array
class PTArray : public PTData
{
public:
    KIT_CREATE_FUNC(PTArray)

    PTArray();
    virtual ~PTArray();

    virtual PTDataType getType() const { return PTDataType::ARRAY; }
    virtual bool serialize(Buffer* buffer) const; 
    virtual bool unserialize(Buffer* Buffer);
    virtual PTData* clone() const;
    virtual std::string toString() const;

    void addData(PTData* data);
    // 设置模版
    void setTemplate(PTData* data);
    void clear();

protected:
    typedef std::vector<PTData*> DataVec;
    DataVec datas_;
    PTData* item_template;
};

// Table
class PTTable : public PTData
{
public:
    KIT_CREATE_FUNC(PTTable)

    virtual ~PTTable();

    virtual PTDataType getType() const { return PTDataType::TABLE; }

    virtual bool serialize(Buffer* buffer) const;
    virtual bool unserialize(Buffer* buffer);
    virtual PTData* clone() const;
    virtual std::string toString() const;

    void addData(PTData* data);
    void delData(PTData* data);
    virtual void clear();

protected:
    typedef std::vector<PTData*> DataVec;
    DataVec datas_;
};

// protocol
class Protocol : public PTTable
{
public:
    KIT_CREATE_FUNC(Protocol)
    Protocol();
    virtual ~Protocol();

    virtual PTDataType getType() const { return PTDataType::PROTOCOL; }

    virtual void init(int32_t pid);

    int32_t getPID() const { return pid_; }
    size_t getBudgetSize() const { return budget_size_; }
    void setBudgetSize(size_t size) { budget_size_ = size; }

    virtual std::string toString() const;
protected:
    int32_t pid_;
    size_t budget_size_ = 64;
};

class PTDataCreator : public Ref
{
public:
    KIT_CREATE_FUNC(PTDataCreator)

    PTDataCreator();
    virtual ~PTDataCreator();

    PTData* createPTData(PTValueType value_type);
private:
    template<typename T>
    PTData* createValue();

    PTData* createVarint();

    PTData* createArray();
    PTData* createTable();

    typedef PTData* (PTDataCreator::* PTDataAPI)();
    PTDataAPI api_table_[toint(PTValueType::MAX)];
};


} // namespace kit

#endif


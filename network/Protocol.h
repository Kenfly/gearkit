#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include <string>
#include <functional>

namespace kit {

enum class PTType : uint8_t
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

inline constexpr int toint(PTType t)
{
    return static_cast<int>(t);
}

// data
class PTData : public Ref
{
public:
    virtual ~PTData() {}
    virtual PTType getType() const { return PTType::NONE; }
    virtual void setType(PTType type) {}

    virtual bool serialize(Buffer* buffer) const = 0;
    virtual bool unserialize(Buffer* buffer) = 0;

    const std::string& getKey() const { return key_; }
    void setKey(const std::string& k) { key_ = k; }

    virtual void resetValue() {}

    virtual PTData* clone() const { return NULL; }

protected:
    std::string key_;
};

// normal value
template<typename T>
class PTValue : public PTData
{
public:
    KIT_CREATE_FUNC(PTValue<T>)
    virtual PTType getType() const { return type_; }
    virtual void setType(PTType type) { type_ = type; }

    const T& getValue() const { return value_; }
    void setValue(const T& v) { value_ = v; }
    void resetValue() { value_ = 0; }

    virtual bool serialize(Buffer* buffer) const;
    virtual bool unserialize(Buffer* buffer);

    virtual PTData* clone() const;
    virtual std::string toString() const;

protected:
    PTType type_;
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

typedef std::vector<PTData*> PTDataVec;

// array
class PTArray : public PTData
{
public:
    KIT_CREATE_FUNC(PTArray)

    PTArray();
    virtual ~PTArray();

    virtual PTType getType() const { return PTType::ARRAY; }

    virtual bool serialize(Buffer* buffer) const; 
    virtual bool unserialize(Buffer* Buffer);
    virtual PTData* clone() const;
    virtual std::string toString() const;
    virtual void resetValue();

    void addData(PTData* data);
    // 设置模版
    void setTemplate(PTData* data);
    PTData* cloneItem();
    void clear();

    const PTDataVec& getDatas() const { return datas_; }
protected:
    PTDataVec datas_;
    PTData* item_template_;
};

// Table
class PTTable : public PTData
{
public:
    KIT_CREATE_FUNC(PTTable)

    virtual ~PTTable();

    virtual PTType getType() const { return PTType::TABLE; }

    virtual bool serialize(Buffer* buffer) const;
    virtual bool unserialize(Buffer* buffer);
    virtual PTData* clone() const;
    virtual std::string toString() const;

    void addData(PTData* data);
    void delData(PTData* data);
    virtual void resetValue();
    virtual void clear();

    const PTDataVec& getDatas() const { return datas_; }
protected:
    PTDataVec datas_;
};

// protocol
class Protocol : public PTTable
{
public:
    KIT_CREATE_FUNC(Protocol)
    Protocol();
    virtual ~Protocol();

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

    PTData* createPTData(PTType type);
private:
    template<typename T>
    PTData* createValue();

    PTData* createVarint();

    PTData* createArray();
    PTData* createTable();

    typedef PTData* (PTDataCreator::* PTDataAPI)();
    PTDataAPI api_table_[toint(PTType::MAX)];
};


} // namespace kit

#endif


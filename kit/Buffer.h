#ifndef __KIT_BUFFER_H__
#define __KIT_BUFFER_H__

#include "Ref.h"
#include "kitsys.h"
#include <string.h>
#include <string>
#include <vector>

// not safe for thread

namespace kit {

class Buffer : public Ref
{
public:
    KIT_CREATE_FUNC(Buffer);

    Buffer();
    virtual ~Buffer();

    bool init(uint32_t size);

    void reset() { write_cur_ = head_; read_cur_ = head_; };
    uint32_t getSize() const;
    uint32_t getMemorySize() const { return mem_size_; }
    uint32_t getWrittenSize() const;
    uint32_t getWritableSize() const;
    uint32_t getReadableSize() const;
    void skipWrite(uint32_t size) { write_cur_ += size; }
    void skipRead(uint32_t size) { read_cur_ += size; }

    bool writeBuffer(const void* buf, uint32_t size);
	bool readBuffer(void* buf, uint32_t size);

	template<typename T>
	bool operator<<(T v);

	template<typename T>
	bool operator>>(T& v);

    template<typename T>
    bool writeVector(const std::vector<T>& vec);
    template<typename T>
    bool readVector(std::vector<T>& vec);

    // varints
    bool writeVar_int32_t(int32_t v);
    bool writeVar_uint32_t(uint32_t v);
    bool writeVar_int64_t(int64_t v);
    bool writeVar_uint64_t(uint64_t v);
    bool readVar_int32_t(int32_t* v);
    bool readVar_uint32_t(uint32_t* v);
    bool readVar_int64_t(int64_t* v);
    bool readVar_uint64_t(uint64_t* v);

    int32_t zigZagDecode32(uint32_t v);
    uint32_t zigZagEncode32(int32_t v);
    int64_t zigZagDecode64(uint64_t v);
    uint64_t zigZagEncode64(int64_t v);

    void debugPrint();
public:
    uint32_t mem_size_;
    char* head_;
    char* tail_;
    char* write_cur_;
    char* read_cur_;
};

inline uint32_t Buffer::getSize() const
{
    return (uint32_t)(tail_ - head_);
}

inline uint32_t Buffer::getWrittenSize() const
{
    return (uint32_t)(write_cur_ - head_);
}

inline uint32_t Buffer::getWritableSize() const
{
    return (uint32_t)(tail_ - write_cur_);
}

inline uint32_t Buffer::getReadableSize() const
{
    return (uint32_t)(write_cur_ - read_cur_);
}

template<typename T>
inline bool Buffer::operator<<(T v)
{
    // TODO: little endian or big endian
	return writeBuffer(&v, sizeof(T));
}

template<>
inline bool Buffer::operator<< <const char*>(const char* v)
{
	uint32_t size = (uint32_t)strlen(v);
    this->writeVar_uint32_t(size);
	return writeBuffer(v, (uint32_t)size);
}

template<typename T>
inline bool Buffer::operator>>(T& v)
{
	char buf[sizeof(T)];
	if(readBuffer(buf, sizeof(T)))
	{
        v = *((T*)buf);
        return true;
    }
    else
        return false;
}

template<>
inline bool Buffer::operator>> <char*>(char*& v)
{
	char* buf = v;
	uint32_t size;
    this->readVar_uint32_t(&size);
	return readBuffer(buf, size);
}

template<>
inline bool Buffer::operator>> <std::string>(std::string& v)
{
    uint16_t size;
    (*this) >> size;
    if (size < 512)
    {
        char buf[512];
        if (!readBuffer(buf, size))
            return false;
        buf[size] = 0;
        v = buf;
    }
    else
    {
        char* buf = new char[size + 1];
        if (!readBuffer(buf, size))
            return false;
        buf[size] = 0;
        v = buf;
        delete [] buf;
    }
    return true;
}

template<typename T>
inline bool Buffer::writeVector(const std::vector<T>& vec)
{
    uint32_t size = static_cast<uint32_t>(vec.size());
    if (!writeVar_uint32_t(size))
        return false;
    for (uint32_t i = 0; i != size; ++i)
    {
        if (!((*this) << vec[i]))
            return false;
    }
    return true;
}

template<typename T>
inline bool Buffer::readVector(std::vector<T>& vec)
{
    uint32_t size = 0;
    if (!readVar_uint32_t(&size))
        return false;
    vec.resize(size);
    for (uint32_t i = 0; i != size; ++i)
    {
        if (!((*this) >> vec[i]))
            return false;
    }
    return true;
}

inline int32_t Buffer::zigZagDecode32(uint32_t v)
{
    return static_cast<int32_t>((v >> 1) ^ (~(v & 1) + 1));
}

inline uint32_t Buffer::zigZagEncode32(int32_t v)
{
    return (static_cast<uint32_t>(v) << 1) ^ static_cast<uint32_t>(v >> 31);
}

inline int64_t Buffer::zigZagDecode64(uint64_t v)
{
    return static_cast<int64_t>((v >> 1) ^ (~(v & 1) + 1));
}

inline uint64_t Buffer::zigZagEncode64(int64_t v)
{
    return (static_cast<uint64_t>(v) << 1) ^ static_cast<uint64_t>(v >> 63);
}

} // namespace kit

#endif

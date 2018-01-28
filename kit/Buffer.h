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

    bool init(size_t size);

    void reset() { write_cur_ = head_; read_cur_ = head_; };
    size_t getSize() const;
    size_t getCapacity() const;
    size_t getWrittenSize() const;
    size_t getWritableSize() const;
	size_t getReadSize() const;
    size_t getReadableSize() const;
    void skipWrite(size_t size) { write_cur_ += size; }
    void skipRead(size_t size) { read_cur_ += size; }

	bool expandBuffer(size_t require_size);
    bool writeBuffer(const void* buf, size_t size);
	bool readBuffer(void* buf, size_t size);

	template<typename T>
	bool operator<<(T v);

	template<typename T>
	bool operator>>(T& v);

    template<typename T>
    bool writeVector(const std::vector<T>& vec);
    template<typename T>
    bool readVector(std::vector<T>& vec);

    // varints
    template<typename T>
    bool writeVaruint(T value);
    bool writeVarint(T value);

    bool writeVar_int32_t(int32_t v);
    bool writeVar_size_t(size_t v);
    bool writeVar_int64_t(int64_t v);
    bool writeVar_uint64_t(uint64_t v);
    bool readVar_int32_t(int32_t* v);
    bool readVar_size_t(size_t* v);
    bool readVar_int64_t(int64_t* v);
    bool readVar_uint64_t(uint64_t* v);

    int32_t zigZagDecode32(size_t v);
    size_t zigZagEncode32(int32_t v);
    int64_t zigZagDecode64(uint64_t v);
    uint64_t zigZagEncode64(int64_t v);

    void debugPrint();
public:
    size_t capacity_;
    uint8_t* head_;
    uint8_t* tail_;
    uint8_t* write_cur_;
    uint8_t* read_cur_;
};

inline size_t Buffer::getSize() const
{
    return static_cast<size_t>(tail_ - head_);
}

inline size_t Buffer::getCapacity() const
{
    return capacity_;
}

inline size_t Buffer::getWrittenSize() const
{
    return static_cast<size_t>(write_cur_ - head_);
}

inline size_t Buffer::getWritableSize() const
{
    return static_cast<size_t>(tail_ - write_cur_);
}

inline size_t Buffer::getReadSize() const
{
	return static_cast<size_t>(read_cur_ - head_);
}

inline size_t Buffer::getReadableSize() const
{
    return static_cast<size_t>(write_cur_ - read_cur_);
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
	size_t size = strlen(v);
    this->writeVaruint(size);
	return writeBuffer(v, (size_t)size);
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
	size_t size;
    this->writeVaruint(&size);
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
    size_t size = static_cast<size_t>(vec.size());
    if (!writeVar_size_t(size))
        return false;
    for (size_t i = 0; i != size; ++i)
    {
        if (!((*this) << vec[i]))
            return false;
    }
    return true;
}

template<typename T>
inline bool Buffer::readVector(std::vector<T>& vec)
{
    size_t size = 0;
    if (!readVar_size_t(&size))
        return false;
    vec.resize(size);
    for (size_t i = 0; i != size; ++i)
    {
        if (!((*this) >> vec[i]))
            return false;
    }
    return true;
}

inline int32_t Buffer::zigZagDecode32(size_t v)
{
    return static_cast<int32_t>((v >> 1) ^ (~(v & 1) + 1));
}

inline size_t Buffer::zigZagEncode32(int32_t v)
{
    return (static_cast<size_t>(v) << 1) ^ static_cast<size_t>(v >> 31);
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

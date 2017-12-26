#ifndef __KIT_BUFFER_H__
#define __KIT_BUFFER_H__

#include "Ref.h"
#include "kitsys.h"
#include <string.h>
#include <string>

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
	Buffer& operator<<(T v);

	template<typename T>
	Buffer& operator>>(T& v);

    // varints
    void writeVarint(int32_t v);
    void writeVarint(uint32_t v);
    void writeVarint(int64_t v);
    void writeVarint(uint64_t v);
    void readVarint(int32_t* v);
    void readVarint(uint32_t* v);
    void readVarint(int64_t* v);
    void readVarint(uint64_t* v);

    int32_t zigZagDecode(uint32_t v);
    uint32_t zigZagEncode(int32_t v);
    int64_t zigZagDecode(uint64_t v);
    uint64_t zigZagEncode(int64_t v);

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
inline Buffer& Buffer::operator<<(T v)
{
    // TODO: little endian or big endian
	writeBuffer(&v, sizeof(T));
	//TODO: ret would be false
	return *this;
}

template<>
inline Buffer& Buffer::operator<< <const char*>(const char* v)
{
	uint16_t size = (uint16_t)strlen(v);
	(*this) << size;
	writeBuffer(v, (uint32_t)size);
	return *this;
}

template<typename T>
inline Buffer& Buffer::operator>>(T& v)
{
	char buf[sizeof(T)];
	if(readBuffer(buf, sizeof(T)))
		v = *((T*)buf);
	return *this;
}

template<>
inline Buffer& Buffer::operator>> <char*>(char*& v)
{
	char* buf = v;
	uint16_t size;
	(*this) >> size;
	readBuffer(buf, size);
	return *this;
}

template<>
inline Buffer& Buffer::operator>> <std::string>(std::string& v)
{
    uint16_t size;
    (*this) >> size;
    if (size < 512)
    {
        char buf[512];
        readBuffer(buf, size);
        buf[size] = 0;
        v = buf;
    }
    else
    {
        char* buf = new char[size + 1];
        readBuffer(buf, size);
        buf[size] = 0;
        v = buf;
        delete [] buf;
    }
    return *this;
}

inline int32_t Buffer::zigZagDecode(uint32_t v)
{
    return static_cast<int32_t>((v >> 1) ^ (~(v & 1) + 1));
}

inline uint32_t Buffer::zigZagEncode(int32_t v)
{
    return (static_cast<uint32_t>(v) << 1) ^ static_cast<uint32_t>(v >> 31);
}

inline int64_t Buffer::zigZagDecode(uint64_t v)
{
    return static_cast<int64_t>((v >> 1) ^ (~(v & 1) + 1));
}

inline uint64_t Buffer::zigZagEncode(int64_t v)
{
    return (static_cast<uint64_t>(v) << 1) ^ static_cast<uint64_t>(v >> 63);
}

} // namespace kit

#endif

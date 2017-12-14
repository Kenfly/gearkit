#ifndef __KIT_BUFFER_H__
#define __KIT_BUFFER_H__

#include "Ref.h"
#include "kitsys.h"
#include <string.h>

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
	bool res = writeBuffer(&v, sizeof(T));
	//to do sothing
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

} // namespace kit

#endif

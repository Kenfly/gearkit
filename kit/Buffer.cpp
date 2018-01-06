#include "Buffer.h"
#include <stdlib.h>
#include <stdio.h>

namespace kit {

Buffer::Buffer()
: mem_size_(0)
, head_(NULL)
, tail_(NULL)
, write_cur_(NULL)
, read_cur_(NULL)
{
}

Buffer::~Buffer()
{
    if (head_)
    {
        free(head_);
    }
}

bool Buffer::init(uint32_t size)
{
    mem_size_ = size;
    head_ = (char*)malloc(sizeof(char) * size);
	tail_ = head_ + size;
	write_cur_ = head_;
	read_cur_ = head_;
    return true;
}

bool Buffer::writeBuffer(const void* buf, uint32_t size)
{
    if(write_cur_ + size <= tail_)
    {
        memcpy(write_cur_, buf, size);
        write_cur_ += size;
        return true;
    }
    uint32_t reuse_size = read_cur_ - head_ + tail_ - write_cur_;
    if(reuse_size >= size)
    {
        uint32_t rsz = getReadableSize();
        memmove(head_, read_cur_, rsz );
        read_cur_ = head_;
        write_cur_ = read_cur_ + rsz;
        memcpy(write_cur_, buf, size);
        write_cur_ += size;
        return true;
    }
    return false;
}

bool Buffer::readBuffer(void* buf, uint32_t size)
{
	if(read_cur_ + size > tail_)
		return false;
	memcpy(buf, read_cur_, size);
	read_cur_ += size;
	return true;
}

void Buffer::debugPrint()
{
    uint32_t size = getWrittenSize();
    printf("[Buffer](%d):\n", size);
    for(uint32_t i = 0; i != size; ++i)
    {
        printf("[%x]", (uint8_t)head_[i]);
    }
    printf("\n");
}

// varints
// TODO: might false
bool Buffer::writeVar_int32_t(int32_t v)
{
    return writeVar_uint32_t(zigZagEncode32(v));
}

bool Buffer::writeVar_uint32_t(uint32_t v)
{
    uint8_t* p = reinterpret_cast<uint8_t*>(write_cur_);
    while (v >= 0x80)
    {
        *p = static_cast<uint8_t>(v | 0x80);
        v >>= 7;
        ++p;
    }
    *p++ = static_cast<uint8_t>(v);
    write_cur_ = reinterpret_cast<char*>(p);
    return true;
}

bool Buffer::writeVar_int64_t(int64_t v)
{
    return writeVar_uint64_t(zigZagEncode64(v));
}

bool Buffer::writeVar_uint64_t(uint64_t v)
{
    uint8_t* p = reinterpret_cast<uint8_t*>(write_cur_);
    while (v >= 0x80)
    {
        *p = static_cast<uint8_t>(v | 0x80);
        v >>= 7;
        ++p;
    }
    *p++ = static_cast<uint8_t>(v);
    write_cur_ = reinterpret_cast<char*>(p);
    return true;
}

bool Buffer::readVar_int32_t(int32_t* v)
{
    uint32_t res;
    if (!readVar_uint32_t(&res))
        return false;
    *v = zigZagDecode32(res);
    return true;
}

bool Buffer::readVar_uint32_t(uint32_t* v)
{
    // TODO: might fail
    const uint8_t* p = reinterpret_cast<const uint8_t*>(read_cur_);
    uint32_t b;
    uint32_t result;
    b = *(p++); result = b; if (!(b & 0x80)) goto done;
    result &= 0x7F;
    b = *(p++); result |= b <<  7; if (!(b & 0x80)) goto done;
    result &= 0x3FFF;
    b = *(p++); result |= b << 14; if (!(b & 0x80)) goto done;
    result &= 0x1FFFFF;
    b = *(p++); result |= b << 21; if (!(b & 0x80)) goto done;
    result &= 0xFFFFFFF;
    b = *(p++); result += b << 28; goto done;

done:
    *v = result;
    read_cur_ = reinterpret_cast<char*>(const_cast<uint8_t*>(p));
    return true;
}

bool Buffer::readVar_int64_t(int64_t* v)
{
    uint64_t res;
    if (!readVar_uint64_t(&res))
        return false;
    *v = zigZagDecode64(res);
    return true;
}

bool Buffer::readVar_uint64_t(uint64_t* v)
{
    // TODO: might fail
    const uint8_t* p = reinterpret_cast<const uint8_t*>(read_cur_);
    uint64_t b;
    uint64_t result;
    b = *(p++); result = b; if (!(b & 0x80)) goto done;
    result &= 0x7F;
    b = *(p++); result |= b <<  7; if (!(b & 0x80)) goto done;
    result &= 0x7FFF;
    b = *(p++); result |= b << 14; if (!(b & 0x80)) goto done;
    result &= 0x7FFFFF;
    b = *(p++); result |= b << 21; if (!(b & 0x80)) goto done;
    result &= 0x7FFFFFFF;
    b = *(p++); result |= b << 28; if (!(b & 0x80)) goto done;
    result &= 0x7FFFFFFFF;
    b = *(p++); result |= b << 35; if (!(b & 0x80)) goto done;
    result &= 0x7FFFFFFFFFF;
    b = *(p++); result |= b << 43; if (!(b & 0x80)) goto done;
    result &= 0x7FFFFFFFFFFFF;
    b = *(p++); result |= b << 50; if (!(b & 0x80)) goto done;
    result &= 0x7FFFFFFFFFFFFFF;
    b = *(p++); result += b << 57; goto done;
done:
    *v = result;
    read_cur_ = reinterpret_cast<char*>(const_cast<uint8_t*>(p));
    return true;
}


} //namespace kit


#include "Buffer.h"
#include <stdlib.h>
#include <stdio.h>

namespace kit {

Buffer::Buffer()
: capacity_(0)
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

bool Buffer::init(size_t size)
{
	capacity_ = size;
    head_ = reinterpret_cast<uint8_t*>(malloc(sizeof(char) * size));
	tail_ = head_ + size;
	write_cur_ = head_;
	read_cur_ = head_;
    return true;
}

bool Buffer::expandBuffer(size_t required_capacity)
{
	size_t requested_capacity = std::max(required_capacity + 64, capacity_ * 2);
	size_t written_size = getWrittenSize();
	size_t read_size = getReadSize();
	void* new_buf = realloc(head_, requested_capacity);
	if (new_buf)
	{
		head_ = reinterpret_cast<uint8_t*>(new_buf);
		tail_ = head_ + requested_capacity;
		write_cur_ = head_ + written_size;
		read_cur_ = head_ + read_size;
        capacity_ = requested_capacity;
		return true;
	} else {
		return false;
	}
}

bool Buffer::reserveBuffer(size_t size)
{
    uint8_t* new_tail = write_cur_ + size;
	if (new_tail <= tail_)
		return true;
    size_t reuse_size = read_cur_ - head_ + tail_ - write_cur_;
    if(reuse_size >= size)
    {
        size_t rsz = getReadableSize();
        memmove(head_, read_cur_, rsz);
        read_cur_ = head_;
        write_cur_ = read_cur_ + rsz;
        return true;
    } 
    if (new_tail <= head_ + capacity_)
    {
        tail_ = new_tail;
        return true;
    }
    return expandBuffer(new_tail - head_);
}

bool Buffer::writeBuffer(const void* buf, size_t size)
{
    if (reserveBuffer(size))
    {
        memcpy(write_cur_, buf, size);
        write_cur_ += size;
        return true;
    }
    return false;
}

bool Buffer::readBuffer(void* buf, size_t size)
{
	if(read_cur_ + size > tail_)
		return false;
	memcpy(buf, read_cur_, size);
	read_cur_ += size;
	return true;
}

std::string Buffer::toString() const
{
    size_t size = getWrittenSize();
    std::string s = "[Buffer][" + std::to_string(getCapacity()) + "]("
        + std::to_string(size) + ")\n";
    for(size_t i = 0; i != size; ++i)
    {
        s += "[" + std::to_string(static_cast<uint8_t>(head_[i])) + "]";
    }
    return s;
}

} //namespace kit


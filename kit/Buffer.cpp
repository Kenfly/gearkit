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
    for(int i = 0; i != size; ++i)
    {
        printf("%c", head_[i]);
    }
    printf("\n");
}

} //namespace kit


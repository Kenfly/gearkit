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

    void reset();
    size_t getSize() const;
    void setSize(size_t size);
    size_t getCapacity() const;
    size_t getWrittenSize() const;
    size_t getWritableSize() const;
	size_t getReadSize() const;
    size_t getReadableSize() const;
    char* writeCur();
    char* readCur();
    void skipWrite(size_t size);
    void skipRead(size_t size);

    bool reserveBuffer(size_t size);
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
    template<typename T>
    bool writeVarint(T value);
    template<typename T>
    bool readVaruint(T& value);
    template<typename T>
    bool readVarint(T& value);

    std::string toString() const;
protected:
    size_t capacity_;
    uint8_t* head_;
    uint8_t* tail_;
    uint8_t* write_cur_;
    uint8_t* read_cur_;
};


} // namespace kit

#include "impl/BufferImpl.h"

#endif


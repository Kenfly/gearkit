namespace kit {

inline void Buffer::reset()
{
    write_cur_ = head_;
    read_cur_ = head_;
}

inline size_t Buffer::getSize() const
{
    return static_cast<size_t>(tail_ - head_);
}

inline void Buffer::setSize(size_t size)
{
    tail_ = head_ + size;
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

inline char* Buffer::writeCur()
{
    return reinterpret_cast<char*>(write_cur_);
}

inline char* Buffer::readCur()
{
    return reinterpret_cast<char*>(read_cur_);
}

inline void Buffer::skipWrite(size_t size)
{
    write_cur_ += size;
}

inline void Buffer::skipRead(size_t size)
{
    read_cur_ += size;
}

template<typename T>
inline bool Buffer::operator<<(const T& v)
{
    // TODO: little endian or big endian
	return writeBuffer(&v, sizeof(T));
}

/*
template<typename T>
inline bool Buffer::operator<<(const T* v)
{
    perror("ERROR Buffer << type!\n");
    return false;
}
*/

template<>
inline bool Buffer::operator<< <std::string>(const std::string& v)
{
    size_t size = v.length();
    writeVaruint<size_t>(size);
    //(*this) << size;
    return writeBuffer(v.c_str(), size);
}

inline bool Buffer::operator<< (const char* v)
{
	size_t size = strlen(v);
    writeVaruint<size_t>(size);
	return writeBuffer(v, size);
}

/*
template<>
inline bool Buffer::operator<< <char*> (char* v)
{
    return this->operator<< <const char*>(v);
}
*/


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
    readVaruint<size_t>(size);
	return readBuffer(buf, size);
}

template<>
inline bool Buffer::operator>> <std::string>(std::string& v)
{
    size_t size;
    this->readVaruint<size_t>(size);
    //(*this) >> size;
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
    size_t size = vec.size();
    if (!writeVaruint<size_t>(size))
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
    if (!readVaruint<size_t>(size))
        return false;
    vec.resize(size);
    for (size_t i = 0; i != size; ++i)
    {
        if (!((*this) >> vec[i]))
            return false;
    }
    return true;
}

// varints
template<typename T>
inline bool Buffer::writeVaruint(T value)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
            "Only unsigned integer types can be written as varuints.");
    uint8_t stack_buf[sizeof(T) * 8 / 7 + 1];
	uint8_t* next_byte = &stack_buf[0];
	do
	{
		*next_byte = (value & 0x7f) | 0x80;
        //printf("____w___[%d]__\n", (*next_byte) & 0x7f);
		++next_byte;
		value >>= 7;
	} while (value);
	*(next_byte - 1) &= 0x7f;
	return writeBuffer(stack_buf, next_byte - stack_buf);
}

template<typename T>
inline bool Buffer::writeVarint(T value)
{
    static_assert(std::is_integral<T>::value && std::is_signed<T>::value,
            "Only signed integer types can be written as varints.");
    using UnsignedT = typename std::make_unsigned<T>::type;
    return writeVaruint((static_cast<UnsignedT>(value) << 1) ^ (value >> (8 * sizeof(T) - 1)));
}

template<typename T>
inline bool Buffer::readVaruint(T& value)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
            "Only signed integer types can be read as varuints.");
    unsigned shift = 0;
    bool has_another_byte;
    value = 0;
    do
    {
        if (read_cur_ >= tail_)
            return false;
        uint8_t byte = *read_cur_;
        //printf("_____[%u]_____\n", byte & 0x7f);
        if (shift < sizeof(T) * 8)
        {
            value |= static_cast<T>(byte & 0x7f) << shift;
            shift += 7;
        }
        has_another_byte = byte & 0x80;
        ++read_cur_;
    } while (has_another_byte);
    return true;
}

// varints
template<typename T>
inline bool Buffer::readVarint(T& value)
{
    static_assert(std::is_integral<T>::value && std::is_signed<T>::value,
            "Only signed integer types can be read as varints.");
    using UnsignedT = typename std::make_unsigned<T>::type;
    UnsignedT unsigned_value;
    if (!readVaruint<UnsignedT>(unsigned_value))
        return false;
    value = static_cast<T>((unsigned_value >> 1) ^ -static_cast<T>(unsigned_value & 1));
    return true;
}

} // namespace 


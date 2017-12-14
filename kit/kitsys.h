#ifndef __KIT_SYS_H__
#define __KIT_SYS_H__

// 开启高精度
#define LINUX_TIME_HIGH_PRECISION 0

/*
 * 跟平能有关的一些函数处理
 *
 */

#include <stdint.h>

#ifdef PLATFORM_LINUX
#endif // PLATFORM_LINUX

#ifdef PLATFORM_WINDOWS
#include <intrin.h>
#endif // PLATFORM_WINDOWS


#ifndef NULL
#define NULL 0
#endif

namespace kit {

// sleep milliseconds
void sleep(uint32_t msec);

// return milliseconds when os started
uint32_t time();

template <typename T>
T min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}

} // namespace ekit
#endif


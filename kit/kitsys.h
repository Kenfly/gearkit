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

#ifdef KIT_DEBUG_MODE

#define DPART(gram) gram

#define DCHECK(gram, msg)                   \
    if (!(gram)) {                          \
        perror(msg);                        \
    }

#else

#define DPART(gram)

#define DCHECK(gram, msg)

#endif

namespace kit {

// sleep milliseconds
void sleep(uint32_t msec);

// return milliseconds when os started
uint32_t time();

} // namespace ekit
#endif


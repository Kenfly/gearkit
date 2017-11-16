#include "kitsys.h"
#include <stdio.h>
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

#ifdef PLATFORM_LINUX
#include <unistd.h>
#include <sys/time.h>
#ifdef LINUX_TIME_HIGH_PRECISION
#include <fcntl.h> 
#include <string.h>
#include <stdlib.h>
#endif
#endif

namespace kit {


void sleep(uint32_t msec)
{
#ifdef PLATFORM_LINUX
	usleep((msec << 10) - (msec << 4) - (msec << 3));
#elif defined PLATFORM_WINDOWS
    Sleep(msec);
#endif
}

#if (defined PLATFORM_LINUX) && LINUX_TIME_HIGH_PRECISION
// 读取时间戳计数寄存器TSC(time stamp counter)
// 自cpu上电以来的秒数 = RDTSC读出的周期数 / cpu主频速率
static inline uint64_t tick()
{
    uint32_t ___L, ___H;
    __asm__ __volatile__("rdtsc" : "=a" ( ___L), "=d" ( ___H) );
    return ((uint64_t)___H) << 32 | ___L;
}

static int64_t get_num_from_sysfile( const char* _name, const char* _head )
{
    if( !_name || ! _head ) return -1;
    char buff[4096] = {0,};
    int32_t fd = open( _name, O_RDONLY );
    if( fd > 0 ) {
        int32_t len = read( fd, buff, sizeof(buff)-1 );
        close(fd);
        buff[len-1] = '\0';
        char* p = strstr( buff, _head);
        if( p ) {
            int32_t j = 0;
            int32_t k = 0;
            char num[11] = {0,};
            bool start = false;
            for( j = 0; j < 50; ++j ) { 
                if( p[j] >= '0' && p[j] <= '9' ) { 
                    num[k++] = p[j];
                    start = true; 
                    if( k >= 11 ) break;
                } else {
                    if( start == true ) { break; }   
                }   
            }   
            return (int64_t)atoll(num); 
        }
    }
    return -1;
}

static uint64_t get_cpu_tick()
{
    char path[64]={0,};
    snprintf( path, sizeof(path), "/proc/cpuinfo" );
    int64_t tick = get_num_from_sysfile( path, "cpu MHz" ); 
    uint64_t res = 0;
    if( tick <= 0 ){
        res = 1995*1000000;
    } else {
        res = (uint64_t)tick*1000000;
    }
    printf("cpu hz:%lld\n", res);
    return res;
}
#endif // PLATFORM_LINUX

uint32_t time()
{
#ifdef PLATFORM_LINUX
#if LINUX_TIME_HIGH_PRECISION
    static int64_t freq;
    if (freq == 0)
        freq = get_cpu_tick();
    return uint32_t(tick() * 1000 / freq);
#else
    static bool has_init = false;
    static timeval begin_time;
    struct timezone tz={0, 0};
    if (!has_init)
    {
        gettimeofday(&begin_time, &tz);
        has_init = true;
    }
    timeval time;
    gettimeofday(&time, &tz);
    return uint32_t((time.tv_sec - begin_time.tv_sec) * 1000.0 + time.tv_usec / 1000.0);
#endif
#elif defined PLATFORM_WINDOWS
    static bool has_init = false;
    static bool use_high_resolution = true;
    static int64_t freq;
    static LARGE_INTEGER begin_time;

    if (!has_init)
    {
        LARGE_INTEGER value;
        if (QueryPerformanceFrequency(&value) == 0)
        {
            use_high_resolution = false;
        }
        else
        {
            freq = int64_t(value.QuadPart / 1000);
            QueryPerformanceCounter(&begin_time);
        }
        has_init = true;
    }

    if (!use_high_resolution)
    {
        return GetTickCount();
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return uint32_t(now.QuadPart - begin_time.QuadPart) / freq;
#endif
}

} // namespace kit


#ifndef __DEBUG_FUNC_H__
#define __DEBUG_FUNC_H__

#include <time.h>
#include <stdio.h>

#define KIT_BEGIN(x) \
    clock_t clock_start_xx##x, clock_finish_xx##x; \
    clock_start_xx##x = clock()

#define KIT_END(x, s) \
    clock_finish_xx##x = clock(); \
    printf(s); \
    printf("%lf\n", (double)(clock_finish_xx##x - clock_start_xx##x) / CLOCKS_PER_SEC)

#define KIT_RUN(len) for (int i = 0; i != len; ++i){}

#endif

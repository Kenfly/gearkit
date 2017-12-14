#ifndef __KIT_LOGGER_H__
#define __KIT_LOGGER_H__

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "Singleton.h"

namespace kit {

enum
{
    LOG_TYPE_LOG,   //普通日志
    LOG_TYPE_ERR,   //错误日志
    LOG_TYPE_DBG,   //调试日志
    LOG_TYPE_SQL,   //数据库日志

    MAX_LOG_TYPE,
};


const int MAX_LOG_BUFFER_LEN = 1024 * 16;

#define g_log (kit::Singleton<kit::Logger>::instance())

#define LOG ( g_log->*( g_log->getAPI( kit::LOG_TYPE_LOG ) ) )
#define DBG ( g_log->*( g_log->getAPI( kit::LOG_TYPE_DBG ) ) )
#define ERR ( g_log->*( g_log->getAPI( kit::LOG_TYPE_ERR ) ) )
#define SQL ( g_log->*( g_log->getAPI( kit::LOG_TYPE_SQL ) ) )

class Logger
{
public:
    Logger();
    ~Logger();

    void init();

    typedef void (Logger::* LoggerAPI)(const char* format, ...);
    
    LoggerAPI getAPI(int t);
    void setAPI(int t, LoggerAPI api);
    
    void writeNone(const char* format, ...) {};

    template<int type>
    void writeLog(const char* format, ...)
    {
        static const char* log_str[] = {"[LOG]", "[ERR]", "[DBG]", "[SQL]"};
        char buffer[MAX_LOG_BUFFER_LEN];
        const char* prestr = log_str[type];
        int len_pre = strlen(prestr);
        strncpy(buffer, prestr, len_pre);
        va_list args;
        va_start(args, format);
        vsnprintf(buffer + len_pre, MAX_LOG_BUFFER_LEN - len_pre, format, args);
        int len_buf = strlen(buffer);
        buffer[len_buf] = '\n';
        buffer[len_buf + 1] = 0;
        printf(buffer);
        va_end(args);
    }
private:

    LoggerAPI api_table_[MAX_LOG_TYPE];
};

} // namespace kit

#endif


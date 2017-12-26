#include "Logger.h"

namespace kit {

Logger::Logger()
{
    api_table_[LOG_TYPE_LOG] = &Logger::writeLog<LOG_TYPE_LOG>;
    api_table_[LOG_TYPE_ERR] = &Logger::writeLog<LOG_TYPE_ERR>;
    api_table_[LOG_TYPE_DBG] = &Logger::writeLog<LOG_TYPE_DBG>;
    api_table_[LOG_TYPE_SQL] = &Logger::writeLog<LOG_TYPE_SQL>;
}

Logger::~Logger()
{
}

bool Logger::baseInit()
{
    return true;
}

Logger::LoggerAPI Logger::getAPI(int t)
{
    if (t >= 0 && t < (int)MAX_LOG_TYPE)
        return api_table_[t];
    else
        return &Logger::writeNone;
}

void Logger::setAPI(int t, Logger::LoggerAPI api)
{
    if (t >= 0 && t < (int)MAX_LOG_TYPE)
        api_table_[t] = api;
}

} // namespace kit

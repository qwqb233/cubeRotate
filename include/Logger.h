//
// Created by qwqb233 on 2025/5/9.
//

#ifndef CPP_TEST_LOGGER_H
#define CPP_TEST_LOGGER_H

#include <cstdarg>
#include <string>

typedef enum
{
    log_error,
    log_warning,
    log_info,
    log_debug
}level;

class Logger {
public:
    Logger() = default;
    ~Logger() = default;

    std::string get_logger(level log_level,const char * fun,const int line,const char * fmt,...);
private:
    std::string log;
};


#endif //CPP_TEST_LOGGER_H

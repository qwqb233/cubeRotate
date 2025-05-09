//
// Created by qwqb233 on 2025/5/9.
//

#include "Logger.h"
#include <iostream>

const char * get_log_level(level log_level)
{
    switch (log_level)
    {
        case log_error:
            return "ERROR:";
            break;
        case log_debug:
            return "DEBUG:";
            break;
        case log_info:
            return "INFO:";
            break;
        case log_warning:
            return "WARN:";
            break;
    }
}
std::string Logger::get_logger(level log_level,const char * fun,const int line,const char * fmt,...) {

    va_list arg;
    va_start(arg,fmt);
    char buf[50] = {0};
    vsnprintf(buf,sizeof(buf),fmt,arg);
    va_end(arg);

    char buffer[100];

    sprintf(buffer,"%s[%s %d] %s\r\n",get_log_level(log_level),fun,line,buf);
    std::string str(buffer);
    this->log.clear();
    this->log.append(buffer);
    return str;
    /*result += "test";
    std::cout<<result << std::endl;*/

}
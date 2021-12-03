#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <iostream>
#include <fstream>

#define log_debug   Log::get_instance().debug
#define log_info    Log::get_instance().info
#define log_warn    Log::get_instance().warn
#define log_error   Log::get_instance().error
#define log_fatal   Log::get_instance().fatal

/*
 *  
 *  log level: lower lever less (but important) log showing :)
 *
 * */

enum {
    LOG_OFF,
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_ALL,
};

class Log{
    Log(int _log_level = LOG_INFO, std::string log_name = "testing.log");
    ~Log();

public:
    void debug(const char *format, ...);
    void info(const char *format, ...);
    void warn(const char *format, ...);
    void error(const char *format, ...);
    void fatal(const char *format, ...);
    void set_log_level(int level);

    static Log& get_instance()
    {
        static Log inst;
        return inst;
    }

private:
    void _log(int level, const char *format, va_list ap);

    enum Param {
        MAX_BUFFER_SIZE = 2048
    };

    std::string log_level_string_get(const int level);
    int _log_level;

    std::ofstream _fp;
    std::string _log_path;
};

/*interface for log level setting*/
inline void log_level_set(int log_level){
    Log::get_instance().set_log_level(log_level);
}

#endif


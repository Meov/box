#include <string.h>
#include <stdarg.h>
#include <iostream>
#include <string>

#include "log.hpp"
#define  TIME_LENGTH  128

Log::Log(int log_level,std::string log_name) :
     _log_level(log_level),
     _log_path(log_name)
{

}

Log::~Log() {
    if(_fp.is_open()){
        std::cout << "close fp" << std::endl;
        _fp.close();
    }
}

void Log::set_log_level(int level) {
    if(level < LOG_ALL || level > LOG_OFF){ 
        _log_level = LOG_INFO;
    }
    _log_level = level;
}

void Log::debug(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    _log(LOG_DEBUG, format, ap);
    va_end(ap);
}

void Log::info(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    _log(LOG_INFO, format, ap);
    va_end(ap);
}

void Log::warn(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    _log(LOG_WARN, format, ap);
    va_end(ap);
}

void Log::error(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    _log(LOG_ERROR, format, ap);
    va_end(ap);
}

void Log::fatal(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    _log(LOG_FATAL, format, ap);
    va_end(ap);
}


std::string Log::log_level_string_get(const int level){
    std::string level_string;
    switch(level){
        case LOG_DEBUG:
            level_string = "DEBUG";
            break;
        case LOG_INFO:
            level_string = "INFO ";
            break;
        case LOG_WARN:
            level_string = "WARN ";
            break;
        case LOG_ERROR:
            level_string = "ERROR";
            break;
        case LOG_FATAL:
            level_string = "FATAL";
            break;
        default:
            break;
    }
    
    return level_string;
}

void Log::_log(int level, const char *format, va_list ap) {
    
    /*only show log level smaller then setted level*/
    if (level<= _log_level) {
        char log_data[MAX_BUFFER_SIZE];
        std::string log_buffer;
        std::string log_level_str;

        va_list ap_printf;
        va_copy(ap_printf, ap);
        
        /*add log time*/
        time_t t = time(NULL);
        char time[TIME_LENGTH];
        strftime(time, sizeof(time) - 1, "[%Y-%m-%d %H:%M:%S]", localtime(&t));
        log_buffer.append(time, strlen(time));
        
        /*add '[' and ']'*/
        log_level_str = log_level_string_get(level);
        log_buffer += " [";
        log_buffer.append(std::move(log_level_str));
        log_buffer += "] ";

        /*add log info*/ 
        memset(log_data,0,MAX_BUFFER_SIZE); //clear log_info bug
        vsnprintf(log_data, MAX_BUFFER_SIZE - 1, format, ap_printf);
        log_buffer.append(log_data,strlen(log_data));

        /*write log to a file*/
        if(!_fp.is_open()){
            _fp.open(_log_path.c_str(),std::ios::app);
            if(!_fp.good()){
                std::cerr << "open log file: "<< _log_path << "error!" << std::endl;
            }
        }

        if (!log_buffer.empty() && log_buffer[log_buffer.length()-1] == '\n') {
            log_buffer.erase(log_buffer.length()-1);
        }

#ifdef DEBUG
        for(int i = 0; i < log_buffer.size(); i++){
            printf("%d ",log_buffer.c_str()[i]);
        }
        printf("\n");
#endif
        _fp << log_buffer << std::endl;
        //_fp.close(); /*close fp in destructor*/

        /*show log on the screen*/
        std::cout << log_buffer << std::endl;
        va_end(ap_printf);

    }
}

#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

#include <time.h>

#if defined(_WIN32) || defined(_WIN64)
	#define OS_WINDOWS
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
	#define OS_POSIX
#else
	#error unsupported platform
#endif


// to set name for logs:
// # define LOG_FILE "filename.txt"

/*
[time] |_  [file:row] [function] : [parameters] [msg]
[time] | |_ [file:row][function] : [parameters] [msg]
[time] | | | [file:row] [parameters] [msg]
[time] | | | [file:row] [W,I,E,] [msg]
[time] | |_| [file:row][function] : [parameters] [msg]


уровни дебага:
	- full: вход выход из методов

logger() - конструктор - вход в метод - параметры входа
	   деструктор - результаты
*/

#define DEFAULT_LOG_FILE "log.txt"

#define TYPE_DEBUG   'D'
#define TYPE_WARNING 'W'
#define TYPE_ERROR   'E'
#define TYPE_INFO    'I'

#define D TYPE_DEBUG  
#define W TYPE_WARNING 
#define E TYPE_ERROR  
#define I TYPE_INFO    

namespace mlog
{

class Logger final
{
public:
    // singleton of Scott Meyers
    static Logger* instance();

    int deep_counter = 0;

    template <class T>
    friend Logger& operator<< (Logger& logger, T t);

private:
    Logger();
    ~Logger();

    std::ofstream file;
};


class _Logs 
{
protected:
    template <class T>
    void write_stack_depth(T& t, int deep);

    std::string get_time();
};


class FunctionLog final : _Logs
{
public:
    FunctionLog(const char* name, const char* file, int line);
    ~FunctionLog();

private:
    std::string function_name;
    std::string file_name;
    std::string row;
};


class __Log final : _Logs
{
public:
    __Log(char type, const char* file, int line);
    ~__Log();

    template <class T>
    friend const __Log& operator<< (const __Log& logger, T t);
};


template <class T>
void mlog::_Logs::write_stack_depth(T& t, int deep)
{
    for (int i = 0; i < deep; ++i)
        t << "| "; 
}

template <class T>
const __Log& operator<< (const __Log& log, T t)
{
    auto logger = Logger::instance();
    (*logger) << t;

    return log;
} 

template <class T>
Logger& operator<< (Logger& logger, T t)
{
    logger.file << t;

    return logger;
} 

}

#define LOG mlog::FunctionLog __function_log__(__FUNCTION__, __FILE__, __LINE__);

//#define __log(ch) mlog::__create_log(ch, __FILE__, __LINE__)
//#define __log() mlog::Log('i', __FILE__, __LINE__)
#define _log(ch) mlog::__Log(ch, __FILE__, __LINE__)

//#define log() make_log(__FILE__, __LINE__)



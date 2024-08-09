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
    static Logger* instance() 
    {
        static Logger inst;
        return &inst;
    } 

    int deep_counter = 0;

    template <class T>
    friend Logger& operator<< (Logger& logger, T t);

private:
    Logger()
    {
    #ifdef LOG_FILE
        file.open(LOG_FILE, std::ios::app);
    #else
        file.open(DEFAULT_LOG_FILE, std::ios::app);
    #endif
    }
    ~Logger()
    {
        file.close();
    }

    std::ofstream file;

};

template <class T>
Logger& operator<< (Logger& logger, T t)
{
    logger.file << t;

    return logger;
} 

class _Logs 
{
protected:
    template <class T>
    void write_stack_depth(T& t, int deep)
    {
        for (int i = 0; i < deep; ++i)
            t << "| "; 
    }

    std::string get_time()
    {
        char buffer[26];
        std::tm tm;
        time_t time = std::time(nullptr);

    #if defined(OS_WINDOWS)
        gmtime_s(&tm, &time);
    #else
        gmtime_r(&time, &tm);
    #endif

        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", &tm);

        return std::string(buffer);
    }

};



class FunctionLog final : _Logs
{
public:
    FunctionLog(const char* name, const char* file, int line) :
        function_name(name), file_name(file), row(std::to_string(line))
    {
        auto logger = Logger::instance();

        (*logger) << "[" << get_time() << "] ";
        write_stack_depth(*logger, logger->deep_counter);

        // enter in function
        (*logger) << "|_ [" << file << ":" << line << "] ["<< function_name << "]" << "\n"; 

        ++logger->deep_counter;
    }

    ~FunctionLog()
    {
        auto logger = Logger::instance();
        --logger->deep_counter;

        (*logger) << "[" << get_time() << "] ";
        write_stack_depth(*logger, logger->deep_counter);

        // exit from function
        (*logger) << "|_| [" << file_name << ":" << std::string(row.size(), '_') << "] ";
        (*logger) << "[" << function_name << "]" << "\n"; 

    }

private:
    std::string function_name;
    std::string file_name;
    std::string row;
};

class Log final : _Logs
{
public:
    Log(char type, const char* file, int line)
    {
        auto logger = Logger::instance();
        (*logger) << "[" << get_time() << "] ";
        write_stack_depth(*logger, logger->deep_counter);
        (*logger) << "| [" << file << ":" << line << "] [" << type << "] : "; 
    }

    ~Log()
    {
        auto logger = Logger::instance();
        (*logger) << "\n";
    }

    template <class T>
    Logger& operator<< (T t)
    {
        auto logger = Logger::instance();
        (*logger) << t;

        return *logger;
    } 
};

inline Log create_log(char type, const char* file, const int line)
{
    return Log(type, file, line);
}

}

#define LOG mlog::FunctionLog __function_log__(__FUNCTION__, __FILE__, __LINE__);

#define __log(ch) mlog::create_log(ch,  __FILE__, __LINE__)
//#define log() make_log(__FILE__, __LINE__)



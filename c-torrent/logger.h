#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <ctime>


// to set name for logs:
// # define LOG_FILE "filename.txt"

/*
|_  [time] [file:row] [function] : [parameters] [msg]
| |_ [time] [file:row][function] : [parameters] [msg]
| | | [time] [file:row] [parameters] [msg]
| |-- [time] [file:row] [W,I,E,] [msg]


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
    Logger& operator<< (T t)
    {
        auto logger = Logger::instance();
        logger->file << t;

        return *this;
    } 

private:
    Logger()
    {
    #ifdef LOG_FILE
        file.open(LOG_FILE, std::ios::app);
    #else
        file.open(DEFAULT_LOG_FILE, std::ios::app);
    #endif
    }

    std::ofstream file;

};

template <class T>
void write_stack_depth(T& t, int deep)
{
    for (int i = 0; i < deep; ++i)
        t << "| "; 
}

class FunctionLog final
{
public:
    FunctionLog(const char* name, const char* file, int line) :
        function_name(name), file_name(file), row(std::to_string(line))
    {
        auto logger = Logger::instance();

        write_stack_depth(*logger, logger->deep_counter);

        // enter in function
        (*logger) << "|_ [" << file << ":" << line << "] ["<< function_name << "]" << "\n"; 

        ++logger->deep_counter;
    }

    ~FunctionLog()
    {
        auto logger = Logger::instance();
        --logger->deep_counter;

        write_stack_depth(*logger, logger->deep_counter);

        // exit from function
        (*logger) << "|- [" << file_name << ":" << std::string(row.size(), '_') << "] ";
        (*logger) << "[" << function_name << "]" << "\n"; 

    }

private:
    std::string function_name;
    std::string file_name;
    std::string row;
};

class Log final
{
public:
    Log(char type, const char* file, int line)
    {
        auto logger = Logger::instance();
        write_stack_depth(*logger, logger->deep_counter);
        (*logger) << "| [" << file << ":" << line << "] [" << type << "] : "; 
    }

    Log(const char* file, int line)
    {
        auto logger = Logger::instance();
        write_stack_depth(*logger, logger->deep_counter);
        (*logger) << "| [" << file << ":" << line << "] [" << TYPE_INFO << "] : "; 
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

Log make_log(char type, const char* file, int line)
{
    return Log(type, file, line);
}

Log make_log(const char* file, int line)
{
    return Log(file, line);
}


#define LOG FunctionLog __function_log__(__FUNCTION__, __FILE__, __LINE__);

#define log(char) make_log(char, __FILE__, __LINE__)
#define log() make_log(__FILE__, __LINE__)


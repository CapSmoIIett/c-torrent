#pragma once

#include <fstream>

// to set name for logs:
// # define LOG_FILE "filename.txt"

/*
[time] [file:row] [type] |_  [function] : [parameters] [msg]
[time] [file:row] [type] | |_ [function] : [parameters] [msg]
[time] [file:row] [type] | |-- [parameters] [msg]
[time] [file:row] [type] | |-- [W,I,E,] [msg]


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

    Logger& operator<< (std::string str)
    {
        auto logger = Logger::instance();
        logger->file << str;

        return *this;
    };

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


class Log final
{
public:
    Log()
    {
        auto logger = Logger::instance();
        ++logger->deep_counter;
    }
    ~Log()
    {
        auto logger = Logger::instance();
        --logger->deep_counter;
    }


    Logger& operator()(char type = TYPE_INFO)
    {
        auto logger = Logger::instance();


        return *logger;
    }

private:

    char type;

};



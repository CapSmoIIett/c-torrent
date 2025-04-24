#include "logger.h"



mlog::Logger* mlog::Logger::instance() 
{
    static mlog::Logger inst;
    return &inst;
} 

mlog::Logger::Logger()
{
#ifdef LOG_FILE
    file.open(LOG_FILE, std::ios::app);
#else
    file.open(DEFAULT_LOG_FILE, std::ios::app);
#endif
}

mlog::Logger::~Logger()
{
    file.close();
}

std::string mlog::_Logs::get_time()
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

mlog::FunctionLog::FunctionLog(const char* name, const char* file, int line) :
    function_name(name), file_name(file), row(std::to_string(line))
{
    auto logger = Logger::instance();

    (*logger) << "[" << get_time() << "] ";
    write_stack_depth(*logger, logger->deep_counter);

    // enter in function
    (*logger) << "|_ [" << file << ":" << line << "] ["<< function_name << "]" << "\n"; 

    ++logger->deep_counter;
}

mlog::FunctionLog::~FunctionLog()
{
    auto logger = Logger::instance();
    --logger->deep_counter;

    (*logger) << "[" << get_time() << "] ";
    write_stack_depth(*logger, logger->deep_counter);

    // exit from function
    (*logger) << "|_| [" << file_name << ":" << std::string(row.size(), '_') << "] ";
    (*logger) << "[" << function_name << "]" << "\n"; 

}

mlog::__Log::__Log(char type, const char* file, int line)
{
    auto logger = Logger::instance();
    (*logger) << "[" << get_time() << "] ";
    write_stack_depth(*logger, logger->deep_counter);
    (*logger) << "| [" << file << ":" << line << "] [" << type << "] : "; 
}

mlog::__Log::~__Log()
{
    auto logger = Logger::instance();
    (*logger) << "\n";
}
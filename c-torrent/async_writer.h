#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <fstream>

class AsyncWriter
{
public:
    AsyncWriter(std::string file_name);
    ~AsyncWriter();

    void write(const std::string& str, size_t pos, size_t size = -1);
    void write(const char* buffer, size_t pos, size_t size);
    void open();
    void close();

private:
    std::mutex mtx;
    std::string name;
    std::ofstream file;

};
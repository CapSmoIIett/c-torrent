#pragma once

#include <fstream>
#include <mutex>
#include <string>

class AsyncWriter
{
public:
    AsyncWriter(std::string file_name) :
        name(file_name)
    {}

    void write(const std::string& str, size_t pos);
    void open();
    void close();

private:
    std::mutex mtx;
    std::string name;

};
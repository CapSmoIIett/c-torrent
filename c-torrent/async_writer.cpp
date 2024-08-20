#include "async_writer.h"


AsyncWriter::AsyncWriter(std::string file_name) :
    name(file_name)
{
    open();
}

AsyncWriter::~AsyncWriter()
{
    close();
}

void AsyncWriter::write(const std::string& str, size_t pos, size_t size)
{
    const std::lock_guard<std::mutex> lock(mtx);

    file.seekp(0, std::ios_base::end);
    size_t len = file.tellp();

    size = -1 != size ? size : str.size();

    if (pos == len)
    {
        file.write(str.data(), size);
        return;
    }

    if (pos > len)
    {
        for (int i = 0; i < pos - len; ++i)
        {
            file.put(0);
        }

        file.write(str.data(), size);

        return;
    }

    file.seekp(pos);
    file.write(str.data(), size);
}

void AsyncWriter::write(const char* buffer, size_t pos, size_t size)
{
    const std::lock_guard<std::mutex> lock(mtx);

    file.seekp(0, std::ios_base::end);
    size_t len = file.tellp();

    if (pos == len)
    {
        file.write(buffer, size);
        return;
    }

    if (pos > len)
    {
        for (int i = 0; i < pos - len; ++i)
        {
            file.put(0);
        }

        file.write(buffer, size);

        return;
    }

    file.seekp(pos);
    file.write(buffer, size);
}

void AsyncWriter::open()
{
    file.open(name, std::ios::binary);
}

void AsyncWriter::close()
{
    file.close();
}

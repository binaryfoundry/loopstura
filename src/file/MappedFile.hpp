#pragma once

#include <string>
#include <stdexcept>

class MappedFile final
{
public:
    MappedFile(const std::string filename);
    ~MappedFile();

    size_t Size()
    {
        return file_size;
    }

    char* Data()
    {
        return mmapped_data;
    }

    size_t Position()
    {
        return position;
    }

    void Advance(size_t offset)
    {
        position += offset;
    }

    template <typename T>
    T Read()
    {
        T* val = (T*)(mmapped_data + position);
        position += sizeof(T);
        return *val;
    }

    template <typename T, size_t S>
    void Read(T* dst)
    {
        T* src = (T*)(mmapped_data + position);
        position += sizeof(T) * S;
        memcpy(dst, src, S);
    }

private:
    int fh;
    size_t file_size;
    char* mmapped_data;
    size_t position = 0;
};

#pragma once

#include <string>
#include <stdexcept>

class MappedFile final
{
public:
    MappedFile(const std::string filename);
    ~MappedFile();

    size_t Size() const
    {
        return file_size;
    }

    char* Data() const
    {
        return mmapped_data;
    }

    size_t Position() const
    {
        return position;
    }

    void Advance(const size_t offset)
    {
        position += offset;
    }

    char* CurrentPointer() const
    {
        return mmapped_data + position;
    }

    template <typename T>
    T Read()
    {
        const T* val = (T*)(mmapped_data + position);
        position += sizeof(T);
        return *val;
    }

    template <typename T, size_t S>
    void Read(T* dst)
    {
        const T* src = (T*)(mmapped_data + position);
        position += sizeof(T) * S;
        memcpy(dst, src, S);
    }

private:
    int fh;
    size_t file_size;
    char* mmapped_data;
    size_t position = 0;
};

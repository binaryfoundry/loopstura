#pragma once

#include "MappedFile.hpp"

#include <fcntl.h>

#ifdef _WIN32
#pragma warning(disable:4996)
#include "winmmap/windows-mmap.h"
#endif

MappedFile::MappedFile(const std::string filename)
{
    struct __stat64 s;
    fh = _open(filename.c_str(), O_RDONLY);
    int status = _fstat64(fh, &s);
    if (status == -1)
    {
        throw std::runtime_error("Failed to open file");
    }
    file_size = s.st_size;
    mmapped_data = static_cast<char*>(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fh, 0));
}

MappedFile::~MappedFile()
{
    munmap(mmapped_data, file_size);
    _close(fh);
}

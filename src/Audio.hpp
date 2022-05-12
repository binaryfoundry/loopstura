#pragma once

#include "file/WAVFile.hpp"

class Audio
{

public:
    Audio();
    virtual ~Audio();

protected:
    std::unique_ptr<WAVFile> wav_file;
};

#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "MappedFile.hpp"

class WAVFile
{
public:
    WAVFile(const std::string path);
    virtual ~WAVFile();

    template<typename T>
    T const ReadSample()
    {
        const T* val = (T*)(data + position);
        position += sizeof(T);
        position %= subchunk2_size;
        return *val;
    }

    uint32_t const SampleRate() const
    {
        return sample_rate;
    }

    uint16_t const NumChannels() const
    {
        return num_channels;
    }

private:
    uint8_t  chunk_id[4] = { 0, 0, 0, 0 };
    uint32_t chunk_size = 0;
    uint8_t  format[4] = { 0, 0, 0, 0 };
    uint8_t  subchunk1_id[4] = { 0, 0, 0, 0 };
    uint32_t subchunk1_size = 0;
    uint16_t audio_format = 0;
    uint16_t num_channels = 0;
    uint32_t sample_rate = 0;
    uint32_t byte_rate = 0;
    uint16_t block_align = 0;
    uint16_t bits_per_sample = 0;
    uint8_t  subchunk2_id[4] = { 0, 0, 0, 0 };
    uint32_t subchunk2_size = 0;

    void ReadHeader();
    std::unique_ptr<MappedFile> file;

    uint32_t position = 0;
    char* data;
};

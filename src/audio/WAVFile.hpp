#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "Waveform.hpp"
#include "../file/MappedFile.hpp"

class WAVFile
{
public:
    std::atomic<double> position = 0;

    WAVFile(const std::string path);
    virtual ~WAVFile();

    template<typename T>
    T const ReadSample()
    {
        const T* val = (T*)(data + position_to_index(position));
        return *val;
    }

    template<typename T>
    T const ReadSample(const double pos)
    {
        const T* val = (T*)(data + position_to_index(pos));
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

    std::shared_ptr<Waveform> waveform;

private:
    inline size_t position_to_index(double pos)
    {
        return (static_cast<size_t>(pos) * num_channels *
            (bits_per_sample / 8)) % subchunk2_size;
    }

    void DrawWaveform();

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
    size_t   channel_size;

    void ReadHeader();
    std::unique_ptr<MappedFile> file;


    char* data;
};

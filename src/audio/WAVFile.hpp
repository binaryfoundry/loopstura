#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "Waveform.hpp"
#include "../MappedFile.hpp"

namespace Application
{
namespace Audio
{

class WAVFile
{
public:
    WAVFile(const std::string path);
    virtual ~WAVFile();

    double const ReadSample(
        const size_t pos,
        const size_t channel);

    uint32_t const SampleRate() const
    {
        return sample_rate;
    }

    uint16_t const NumChannels() const
    {
        return num_channels;
    }

    size_t const ChannelSize() const
    {
        return channel_size;
    }

    std::shared_ptr<Waveform> waveform;

private:
    inline size_t position_to_index(
        const size_t pos,
        const size_t channel)
    {
        const size_t index = pos * num_channels * (bits_per_sample / 8);
        const size_t channel_offset = channel * (bits_per_sample / 8);
        return (index + channel_offset) % subchunk2_size;
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

}
}

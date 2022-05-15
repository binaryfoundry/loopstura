#include "WAVFile.hpp"

uint16_t swap_endian16(const uint16_t val)
{
    return (val << 8) | (val >> 8);
}

uint16_t swap_endian32(const uint32_t val)
{
     return (val << 24) | ((val << 8) & 0x00ff0000) |
        ((val >> 8) & 0x0000ff00) | (val >> 24);
}

WAVFile::WAVFile(const std::string path)
{
    file = std::make_unique<MappedFile>(path);

    ReadHeader();
}

WAVFile::~WAVFile()
{
}

void WAVFile::ReadHeader()
{
    file->Read<uint8_t, 4>(&chunk_id[0]);

    if (strcmp((const char*)chunk_id, "RIFF"))
    {
        throw std::runtime_error("NOT_WAV_FILE_ERROR");
    }

    chunk_size = file->Read<uint32_t>();

    file->Read<uint8_t, 4>(&format[0]);

    if (strcmp((const char*)format, "WAVE"))
    {
        throw std::runtime_error("FMT_ERROR");
    }

    file->Read<uint8_t, 4>(&subchunk1_id[0]);

    if (strcmp((const char*)subchunk1_id, "fmt "))
    {
        throw std::runtime_error("FMT_ID_ERROR");
    }

    subchunk1_size = file->Read<uint32_t>();
    audio_format = file->Read<uint16_t>();
    num_channels = file->Read<uint16_t>();
    sample_rate = file->Read<uint32_t>();
    byte_rate = file->Read<uint32_t>();
    block_align = file->Read<uint16_t>();
    bits_per_sample = file->Read<uint16_t>();

    file->Read<uint8_t, 4>(&subchunk2_id[0]);
    subchunk2_size = file->Read<uint32_t>();

    data = file->CurrentPointer();

    return;
}

void WAVFile::Draw(std::shared_ptr<Waveform> waveform, const double scale)
{

    for (size_t i = 0; i < waveform->Size(); ++i)
    {
        const float t = static_cast<float>(i) / sample_rate;
        waveform->x_data[i] = t;
        waveform->min_data[i] = 0;
        waveform->max_data[i] = 0;
    }

    const size_t count = static_cast<size_t>(waveform->Size() * scale);
    const double start = scale * static_cast<int64_t>(position / scale);

    constexpr int16_t max_int16_t = std::numeric_limits<int16_t>::max();

    for (size_t i = 0; i < count; ++i)
    {
        size_t j = static_cast<size_t>(i / scale);
        float sample = static_cast<float>(ReadSample<int16_t>(start + i)) / max_int16_t;
        waveform->max_data[j] = std::max(sample, waveform->max_data[j]);
        waveform->min_data[j] = std::min(sample, waveform->min_data[j]);
    }
}

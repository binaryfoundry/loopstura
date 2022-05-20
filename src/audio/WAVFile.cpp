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

    const float display_scaling = 500.0f;

    const size_t waveform_samples = static_cast<size_t>(
        (channel_size / display_scaling) * (44100.0 / sample_rate));

    waveform = std::make_unique<Waveform>(waveform_samples);

    DrawWaveform();
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

    channel_size = subchunk2_size / (num_channels * (bits_per_sample / 8));

    data = file->CurrentPointer();

    return;
}

void WAVFile::DrawWaveform()
{
    const double scale = static_cast<double>(waveform->Size()) / channel_size;

    constexpr int16_t max_int16_t = std::numeric_limits<int16_t>::max();
    size_t j_last = 0;
    size_t bin_sample_count = 0;

    float total_max = 0;
    float total_min = 0;

    for (size_t i = 0; i < channel_size; i++)
    {
        // Position in waveform
        const size_t j = static_cast<size_t>(scale * i);

        // Position in file
        const double k = static_cast<double>(i) * (bits_per_sample / 8);

        const float sample = static_cast<float>(ReadSample<int16_t>(k)) / max_int16_t;

        const float s_max = std::min(sample, 0.0f);
        const float s_min = std::max(sample, 0.0f);

        waveform->max_data[j] += s_max * s_max;
        waveform->min_data[j] += s_min * s_min;

        bin_sample_count++;

        if (j > j_last)
        {
            const float bin_max = sqrtf(waveform->max_data[j] / bin_sample_count);
            const float bin_min = sqrtf(waveform->min_data[j] / bin_sample_count);

            waveform->max_data[j] = bin_max;
            waveform->min_data[j] = bin_min;

            total_max = std::max(bin_max, total_max);
            total_min = std::max(bin_min, total_min);

            j_last = j;
            bin_sample_count = 0;
        }
    }

    // Normalize
    for (size_t i = 0; i < waveform->Size(); i++)
    {
        waveform->max_data[i] = waveform->max_data[i] * total_max;
        waveform->min_data[i] = -(waveform->min_data[i] * total_min);
    }
}

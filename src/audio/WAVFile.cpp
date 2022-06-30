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

double const WAVFile::ReadSample(const double pos)
{
    const size_t num_bytes = bits_per_sample / 8;
    char* src = data + position_to_index(pos);

    int8_t val8 = 0;
    int16_t val16 = 0;
    int32_t val32 = 0;
    int64_t val64 = 0;

    switch (num_bytes)
    {
    case 1:
        memcpy(&val8, src, 1);
        return static_cast<double>(val8);
        break;
    case 2:
        memcpy(&val16, src, 2);
        return static_cast<double>(val16);
        break;
    case 4:
        memcpy(&val32, src, 4);
        return static_cast<double>(val32);
        break;
    case 8:
        memcpy(&val64, src, 8);
        return static_cast<double>(val64);
        break;
    default:
        throw std::runtime_error("Invalid number of bytes in int");
    };
    return 0.0;
}

void WAVFile::DrawWaveform()
{
    const double scale = static_cast<double>(waveform->Size()) / channel_size;

    constexpr int16_t max_int16_t = std::numeric_limits<int16_t>::max();
    size_t j_last = 0;
    size_t bin_sample_count = 0;

    float global_max = 0;
    float global_min = 0;

    auto& wmax = waveform->max_data;
    auto& wmin = waveform->min_data;

    for (size_t i = 0; i < channel_size; i++)
    {
        // Position in waveform
        const size_t j = static_cast<size_t>(scale * i);

        float s_max = 0.0f;
        float s_min = 0.0f;

        for (size_t k = 0; k < num_channels; k++)
        {
            const double pos = static_cast<double>(i + k);
            const float sample =
                static_cast<float>(ReadSample(pos)) /
                static_cast<float>(max_int16_t);

            s_max = std::min(sample, 0.0f);
            s_min = std::max(sample, 0.0f);
        }

        wmax[j] += s_max * s_max;
        wmin[j] += s_min * s_min;

        bin_sample_count++;

        if (j > j_last)
        {
            wmax[j_last] = sqrtf(wmax[j_last] / bin_sample_count);
            wmin[j_last] = sqrtf(wmin[j_last] / bin_sample_count);

            global_max = std::max(wmax[j_last], global_max);
            global_min = std::max(wmin[j_last], global_min);

            j_last = j;
            bin_sample_count = 0;
        }
    }

    // Normalize
    for (size_t i = 0; i < waveform->Size(); i++)
    {
        wmax[i] = wmax[i] / global_max;
        wmin[i] = -wmin[i] / global_min;
    }
}

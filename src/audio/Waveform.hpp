#pragma once

#include <vector>
#include <memory>

#include "../Texture.hpp"

namespace Application
{
namespace Audio
{

class Waveform
{
private:
    size_t size;

public:
    Waveform(
        const size_t size);
    Waveform(const Waveform&) = delete;

    std::vector<float> x_data;
    std::vector<float> min_data;
    std::vector<float> max_data;
    size_t const Size() const
    {
        return size;
    }

    void Resize(
        const size_t size);

    void Fill(
        const double position,
        const float speed,
        std::vector<float>& window);
};

}
}

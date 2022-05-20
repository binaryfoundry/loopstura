#pragma once

#include <vector>
#include <memory>

class Waveform
{
private:
    size_t size;

    void Slice(double position, std::vector<float>& data, std::vector<float>& window);

public:
    Waveform(
        const size_t size);

    std::vector<float> x_data;
    std::vector<float> min_data;
    std::vector<float> max_data;
    size_t const Size() const
    {
        return size;
    }

    void Resize(const size_t size);

    void FillX(double position, std::vector<float>& window_x);
    void FillYMax(double position, std::vector<float>& window_y_max);
    void FillYMin(double position, std::vector<float>& window_y_min);
};

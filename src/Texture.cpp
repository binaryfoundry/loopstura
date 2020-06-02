#include "Texture.hpp"

namespace Application
{
    template <>
    Texture<TexureDataByteRGBA>::Texture(
        uint32_t width,
        uint32_t height) :
        width(width),
        height(height),
        pitch(4),
        data(std::make_shared<std::vector<TexureDataByteRGBA>>())
    {
        Initialize();
    }

    template <>
    Texture<TexureDataFloatRGBA>::Texture(
        uint32_t width,
        uint32_t height) :
        width(width),
        height(height),
        pitch(4),
        data(std::make_shared<std::vector<TexureDataFloatRGBA>>())
    {
        Initialize();
    }

    template <typename T>
    Texture<T>::~Texture()
    {
    }

    template <typename T>
    void Texture<T>::Initialize()
    {
        data->resize(
            width * height * pitch);
    }

    template class Texture<TexureDataByteRGBA>;
    template class Texture<TexureDataFloatRGBA>;
}


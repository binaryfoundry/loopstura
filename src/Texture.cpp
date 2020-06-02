#include "Texture.hpp"

namespace Application
{
    template <>
    Texture<TextureDataByteRGBA>::Texture(
        uint32_t width,
        uint32_t height) :
        width(width),
        height(height),
        pitch(4),
        data(std::make_shared<std::vector<TextureDataByteRGBA>>())
    {
        Initialize();
    }

    template <>
    Texture<TextureDataFloatRGBA>::Texture(
        uint32_t width,
        uint32_t height) :
        width(width),
        height(height),
        pitch(4),
        data(std::make_shared<std::vector<TextureDataFloatRGBA>>())
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

    template <typename T>
    void Texture<T>::Invalidate()
    {
        invalidated = true;
    }

    template class Texture<TextureDataByteRGBA>;
    template class Texture<TextureDataFloatRGBA>;
}


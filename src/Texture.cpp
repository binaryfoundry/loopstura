#include "Texture.hpp"

namespace Application
{
    template <>
    Texture<TextureDataByteRGBA>::Texture(
        uint32_t width,
        uint32_t height,
        std::shared_ptr<std::vector<TextureDataByteRGBA>> data) :
        width(width),
        height(height),
        pitch(4),
        data(data)
    {
    }

    template <>
    Texture<TextureDataFloatRGBA>::Texture(
        uint32_t width,
        uint32_t height,
        std::shared_ptr<std::vector<TextureDataFloatRGBA>> data) :
        width(width),
        height(height),
        pitch(4),
        data(data)
    {
    }

    template <typename T>
    Texture<T>::~Texture()
    {
    }

    template <typename T>
    void Texture<T>::Invalidate()
    {
        invalidated = true;
    }

    template class Texture<TextureDataByteRGBA>;
    template class Texture<TextureDataFloatRGBA>;
}


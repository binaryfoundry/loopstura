#include "Texture.hpp"

namespace Application
{
namespace Rendering
{
    template <>
    Texture<TextureDataByte>::Texture(
        uint32_t width,
        uint32_t height,
        TextureDataRGBA8Ptr data) :
        width(width),
        height(height),
        pitch(4),
        data(data)
    {
    }

    template <>
    Texture<TextureDataFloat>::Texture(
        uint32_t width,
        uint32_t height,
        TextureDataRGBA32FPtr data) :
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

    template class Texture<TextureDataByte>;
    template class Texture<TextureDataFloat>;
}
}

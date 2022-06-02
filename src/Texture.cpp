#include "Texture.hpp"

namespace Application
{
namespace Rendering
{
    template <>
    TextureTyped<TextureDataByte>::TextureTyped(
        uint32_t width,
        uint32_t height,
        TextureDataRGBA8Ptr data) :
        Texture(
            width, height, 4, sizeof(uint8_t)),
        data(data)
    {
    }

    template <>
    TextureTyped<TextureDataFloat>::TextureTyped(
        uint32_t width,
        uint32_t height,
        TextureDataRGBA32FPtr data) :
        Texture(
            width, height, 4, sizeof(float)),
        data(data)
    {
    }

    template <typename T>
    TextureTyped<T>::~TextureTyped()
    {
    }

    template <typename T>
    void TextureTyped<T>::Invalidate()
    {
        invalidated = true;
    }

    template class TextureTyped<TextureDataByte>;
    template class TextureTyped<TextureDataFloat>;
}
}

#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
namespace Rendering
{
    using TextureDataByte = uint8_t;
    using TextureDataFloat = float;

    class Texture
    {
    protected:
        bool invalidated = true;

    public:
        const uint32_t width = 0;
        const uint32_t height = 0;
        const uint8_t pitch = 0;
        const size_t type = 0;

        virtual void Update() = 0;

        Texture(
            uint32_t width,
            uint32_t height,
            uint8_t pitch,
            size_t type) :
            width(width),
            height(height),
            pitch(pitch),
            type(type)
        {
        }
    };

    using TexturePtr = std::shared_ptr<Texture>;

    template <typename T>
    class TextureTyped : public Texture
    {
    protected:
        TextureTyped(
            uint32_t width,
            uint32_t height,
            std::shared_ptr<std::vector<T>> data);

    public:
        virtual ~TextureTyped();

        const std::shared_ptr<std::vector<T>> data;

        void Invalidate();
    };

    using TextureDataRGBA8Ptr = std::shared_ptr<std::vector<TextureDataByte>>;
    using TextureDataRGBA32FPtr = std::shared_ptr<std::vector<TextureDataFloat>>;

    using TextureRGBA8Ptr = std::shared_ptr<TextureTyped<TextureDataByte>>;
    using TextureRGBA32FPtr = std::shared_ptr<TextureTyped<TextureDataFloat>>;
}
}

#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
    using TextureDataByteRGBA = uint8_t;
    using TextureDataFloatRGBA = float;

    template <typename T>
    class Texture
    {
    protected:
        bool invalidated = true;

    public:
        Texture(
            uint32_t width,
            uint32_t height,
            std::shared_ptr<std::vector<T>> data);

        virtual ~Texture();

        const uint32_t width = 0;
        const uint32_t height = 0;
        const uint8_t pitch = 0;

        const std::shared_ptr<std::vector<T>> data;

        void Invalidate();

        virtual void Update() = 0;
    };
}

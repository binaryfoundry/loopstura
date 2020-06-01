#pragma once

#include <stdint.h>
#include <vector>

namespace Application
{
    class Renderer
    {
    protected:
        Renderer(uint32_t width, uint32_t height);

        uint32_t width = 0;
        uint32_t height = 0;

        const uint32_t texture_width = 512;
        const uint32_t texture_height = 512;
        std::vector<uint8_t> texture_data;

    public:
        virtual ~Renderer();

        virtual void Render() = 0;

        void SetSize(uint32_t width, uint32_t height);
    };
}

#pragma once

#include <stdint.h>

namespace Application
{
    class Renderer
    {
    protected:
        Renderer(uint32_t width, uint32_t height);

        uint32_t width = 0;
        uint32_t height = 0;

    public:
        virtual ~Renderer();

        virtual void Render() = 0;

        void SetSize(uint32_t width, uint32_t height);
    };
}

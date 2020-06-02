#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

#include "Texture.hpp"
#include "rendering/InstanceBasic.hpp"

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

        virtual void Draw() = 0;

        std::shared_ptr<Rendering::InstanceBasic> quad_instance;

        void Resize(uint32_t width, uint32_t height);
    };
}

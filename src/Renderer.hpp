#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <functional>

#include "Context.hpp"
#include "Texture.hpp"
#include "rendering/InstanceBasic.hpp"

namespace Application
{
    class Renderer
    {
    protected:
        Renderer(
            ContextPtr context,
            std::function<void()> swap_buffers,
            uint32_t width,
            uint32_t height);

        const ContextPtr context;

        const std::function<void()> swap_buffers;

        uint32_t width = 0;
        uint32_t height = 0;

    public:
        virtual ~Renderer();

        void Resize(uint32_t width, uint32_t height);

        virtual void Begin() = 0;
        virtual void End() = 0;

        virtual std::shared_ptr<Texture<TextureDataByteRGBA>> MakeTexture(
            uint32_t width,
            uint32_t height) = 0;

        virtual std::shared_ptr<Rendering::InstanceBasic> MakeInstanceBasic(
            ContextPtr context,
            std::vector<float>& vertices,
            std::vector<uint32_t>& indices,
            std::shared_ptr<Texture<TextureDataByteRGBA>> texture) = 0;
    };
}

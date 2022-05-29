#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "Context.hpp"
#include "Texture.hpp"
#include "Quad.hpp"

namespace Application
{
namespace Rendering
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

        std::vector<QuadPtr> quads;

    public:
        virtual ~Renderer();

        RenderState state;

        void Resize(
            uint32_t width,
            uint32_t height);

        void RegisterQuad(QuadPtr quad);

        virtual void Draw(RenderState state) = 0;

        virtual TextureRGBA8Ptr MakeTexture(
            uint32_t width,
            uint32_t height) = 0;

        virtual TextureRGBA8Ptr MakeTexture(
            std::string file) = 0;
    };
}
}

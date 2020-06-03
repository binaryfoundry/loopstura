#pragma once

#include "../Renderer.hpp"
#include "../Texture.hpp"

#include "GL.hpp"
#include "GLInstance.hpp"

#include "rendering/GLInstanceBasic.hpp"

#include <functional>
#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
namespace OpenGL
{
    class GLRenderer : public Application::Renderer
    {
    private:
        const std::function<void()> swap_buffers;

        std::shared_ptr<Texture<TextureDataByteRGBA>> MakeTexture(
            uint32_t width,
            uint32_t height);

    public:
        GLRenderer(
            uint32_t width,
            uint32_t height,
            std::function<void()> swap_buffers);
        virtual ~GLRenderer();

        void Begin();
        void End();
    };
}
}

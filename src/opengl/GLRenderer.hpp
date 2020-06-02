#pragma once

#include "../Renderer.hpp"
#include "../Texture.hpp"

#include "GL.hpp"
#include "GLInstance.hpp"

#include "instances/GLInstanceBasic.hpp"

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

        GLuint shader_program_basic = 0;

        std::shared_ptr<GLInstanceBasic> quad_instance;

    public:
        GLRenderer(
            uint32_t width,
            uint32_t height,
            std::function<void()> swap_buffers);
        virtual ~GLRenderer();

        void Render();

        std::shared_ptr<Texture<TextureDataByteRGBA>> MakeTexture(
            uint32_t width,
            uint32_t height);
    };
}
}

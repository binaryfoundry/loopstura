#pragma once

#include "../Renderer.hpp"
#include "../Texture.hpp"

#include "GL.hpp"

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

        GLuint quad_vertex_buffer = 0;
        GLuint quad_index_buffer = 0;

        //GLuint vao = 0;

        GLuint shader = 0;
        GLuint sampler_state = 0;
        GLuint texture_uniform_location = 0;

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

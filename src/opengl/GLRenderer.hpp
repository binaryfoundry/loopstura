#pragma once

#include "../Renderer.hpp"

#include "GL.hpp"

#include <functional>

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

    public:
        GLRenderer(
            uint32_t width,
            uint32_t height,
            std::function<void()> swap_buffers);
        virtual ~GLRenderer();

        void Render();
    };
}
}

#pragma once

#include "../GL.hpp"
#include "../GLStream.hpp"
#include "../GLInstance.hpp"

#include <memory>

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    class GLEnvironmentInstance : public GLInstance
    {
    protected:
        std::shared_ptr<GLStream<float>> quad_vertices;
        std::shared_ptr<GLStream<uint32_t>> quad_indices;

        GLuint gl_shader_program = 0;

        GLuint gl_projection_uniform_location = 0;
        GLuint gl_view_uniform_location = 0;
        GLuint gl_viewport_uniform_location = 0;

        GLuint gl_sampler_state = 0;

    public:
        GLEnvironmentInstance(
            std::shared_ptr<GLStream<float>> quad_vertices,
            std::shared_ptr<GLStream<uint32_t>> quad_indices);

        virtual ~GLEnvironmentInstance();

        void Draw(RenderState state);

        void Bind(RenderState state);
        void Unbind();
    };
}
}
}

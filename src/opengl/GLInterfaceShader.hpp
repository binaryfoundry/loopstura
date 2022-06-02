#pragma once

#include "GL.hpp"
#include "GLStream.hpp"
#include "GLShader.hpp"

#include "../DisplayNode.hpp"

#include <memory>

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    class GLInterfaceShader : public GLShader
    {
    private:
        std::shared_ptr<GLStream<float>> quad_vertices;
        std::shared_ptr<GLStream<uint32_t>> quad_indices;

        GLuint gl_quad_shader_program;
        GLuint gl_quad_projection_uniform_location = 0;
        GLuint gl_quad_model_uniform_location = 0;
        GLuint gl_quad_view_uniform_location = 0;
        GLuint gl_quad_viewport_uniform_location = 0;
        GLuint gl_quad_texture_uniform_location = 0;
        GLuint gl_quad_texture_blend_uniform_location = 0;
        GLuint gl_quad_sdf_function_location = 0;
        GLuint gl_quad_outline_margin_function_location = 0;
        GLuint gl_quad_brightness_uniform_location = 0;
        GLuint gl_quad_gradient_uniform_location = 0;
        GLuint gl_quad_gradient_0_uniform_location = 0;
        GLuint gl_quad_gradient_1_uniform_location = 0;

        GLuint gl_quad_sampler_state = 0;

    public:
        GLInterfaceShader(
            std::shared_ptr<GLStream<float>> quad_vertices,
            std::shared_ptr<GLStream<uint32_t>> quad_indices);

        virtual ~GLInterfaceShader();

        void Draw(DisplayNode* node);

        void Bind(RenderState state);
        void Unbind();
    };
}
}
}

#pragma once

#include "../GL.hpp"
#include "../GLStream.hpp"
#include "../GLInstance.hpp"

#include "../../DisplayNode.hpp"

#include <memory>

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    class GLInterfaceInstance : public GLInstance
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
        GLuint gl_quad_brightness_uniform_location = 0;
        GLuint gl_quad_gradient_uniform_location = 0;
        GLuint gl_quad_gradient_0_uniform_location = 0;
        GLuint gl_quad_gradient_1_uniform_location = 0;
        GLuint gl_quad_alpha_margin_uniform_location = 0;
        GLuint gl_quad_nonlinearity_uniform_location = 0;
        GLuint gl_quad_texscale_uniform_location = 0;

        GLuint gl_quad_sampler_state = 0;

    public:
        GLInterfaceInstance(
            std::shared_ptr<GLStream<float>> quad_vertices,
            std::shared_ptr<GLStream<uint32_t>> quad_indices);

        virtual ~GLInterfaceInstance();

        void Draw(RenderState state, DisplayNode* node);

        void Bind(RenderState state);
        void Unbind();
    };
}
}
}

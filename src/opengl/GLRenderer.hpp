#pragma once

#include "GL.hpp"
#include "GLStream.hpp"
#include "GLImgui.hpp"

#include "../Context.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"

#include <functional>
#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

#include "GLStream.hpp"
#include "../properties/Property.hpp"

using namespace Application::Properties;

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    class GLRenderer : public Renderer
    {
    protected:
        ContextPtr context;

        TextureRGBA8Ptr MakeTexture(
            uint32_t width,
            uint32_t height) override;

        TextureRGBA8Ptr MakeTexture(
            std::string file) override;

        std::unique_ptr<GLImgui> imgui;

        TextureRGBA8Ptr quad_texture;
        std::unique_ptr<GLStream<float>> quad_vertices;
        std::unique_ptr<GLStream<uint32_t>> quad_indices;
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

        void DrawNode(DisplayNode* node);
        void DrawNodes(RenderState state, DisplayNode* node);

    public:
        GLRenderer(
            ContextPtr context,
            std::function<void()> swap_buffers,
            uint32_t width,
            uint32_t height);
        virtual ~GLRenderer();

        void Draw(RenderState state) override;
    };
}
}
}

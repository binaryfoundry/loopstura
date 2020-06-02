#include "GLRenderer.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>

#include "GLTexture.hpp"
#include "GLShader.hpp"

namespace Application
{
namespace OpenGL
{
    std::vector<float> quad_vertices =
    {
        -1.0f,  1.0f, 0.0f,
         1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f,
         1.0f,  1.0f, 0.0f,
         0.0f,  0.0f
    };

    std::vector<uint32_t> quad_indices =
    {
         0, 1, 2, 2, 3, 0
    };

    GLRenderer::GLRenderer(
        uint32_t width,
        uint32_t height,
        std::function<void()> swap_buffers) :
        Application::Renderer(
            width,
            height),
        swap_buffers(swap_buffers)
    {
        shader_program_basic = LinkShader(
            vertex_shader_string_basic_texture,
            fragment_shader_string_basic_texture);

        auto quad_texture = MakeTexture(
            512,
            512);

        quad_instance = std::make_shared<GLInstanceBasic>(
            shader_program_basic,
            quad_vertices,
            quad_indices,
            quad_texture);
    }

    GLRenderer::~GLRenderer()
    {
    }

    std::shared_ptr<Texture<TextureDataByteRGBA>> GLRenderer::MakeTexture(
        uint32_t width,
        uint32_t height)
    {
        return std::make_shared<GLTexture<TextureDataByteRGBA>>(
            width, height);
    }

    void GLRenderer::Begin()
    {
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glViewport(
            0,
            0,
            width,
            height);

        glClearColor(
            0, 1, 1, 1);

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT);
    }

    void GLRenderer::End()
    {
        swap_buffers();
    }
}
}

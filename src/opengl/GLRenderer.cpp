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
    GLRenderer::GLRenderer(
        ContextPtr context,
        std::function<void()> swap_buffers,
        uint32_t width,
        uint32_t height) :
        Application::Renderer(
            context,
            swap_buffers,
            width,
            height)
    {
        GLInstanceBasic::Initialise();

        auto quad_texture = MakeTexture(
            512,
            512);

        quad_instance = std::make_shared<GLInstanceBasic>(
            context,
            quad_vertices,
            quad_indices,
            quad_texture);
    }

    GLRenderer::~GLRenderer()
    {
        GLInstanceBasic::Destroy();
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
            0, 0, 0, 1);

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

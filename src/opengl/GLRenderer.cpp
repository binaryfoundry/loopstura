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
            height),
        context(context)
    {
        GLInstanceBasic::Initialise();
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
            width,
            height);
    }

    std::shared_ptr<Texture<TextureDataByteRGBA>> GLRenderer::MakeTexture(
        std::string file)
    {
        uint8_t bpp;
        uint32_t tex_width, tex_height;

        std::shared_ptr<std::vector<uint8_t>> data =
            std::make_shared<std::vector<uint8_t>>();

        context->load_texture_2d(file, bpp, tex_width, tex_height, data);

        // TODO check matching BPP

        return std::make_shared<GLTexture<TextureDataByteRGBA>>(
            width,
            height);
    }

    std::shared_ptr<Rendering::InstanceBasic> GLRenderer::MakeInstanceBasic(
        ContextPtr context,
        std::vector<float>& vertices,
        std::vector<uint32_t>& indices,
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture)
    {
        return std::make_shared<GLInstanceBasic>(
            context,
            vertices,
            indices,
            texture);
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

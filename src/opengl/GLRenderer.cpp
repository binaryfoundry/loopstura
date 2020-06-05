#include "GLRenderer.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>

#include "GLTexture.hpp"
#include "GLStream.hpp"

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
        GLInstanceUI::Initialise();
    }

    GLRenderer::~GLRenderer()
    {
        GLInstanceUI::Destroy();
    }

    TextureRGBA8Ptr GLRenderer::MakeTexture(
        uint32_t width,
        uint32_t height)
    {
        std::shared_ptr<std::vector<uint8_t>> data =
            std::make_shared<std::vector<uint8_t>>();

        data->resize(
            width * height * 4);

        return std::make_shared<GLTexture<TextureDataByte>>(
            width,
            height,
            data);
    }

    TextureRGBA8Ptr GLRenderer::MakeTexture(
        std::string file)
    {
        uint8_t bpp = 0;
        uint32_t tex_width = 0, tex_height = 0;

        std::shared_ptr<std::vector<uint8_t>> data =
            std::make_shared<std::vector<uint8_t>>();

        context->load_texture_2d(
            file, bpp, tex_width, tex_height, data);

        // TODO check matching BPP

        auto new_texture = std::make_shared<GLTexture<TextureDataByte>>(
            tex_width,
            tex_height,
            data);

        return new_texture;
    }

    VertexStreamPtr GLRenderer::MakeVertexStream(
        std::initializer_list<float>& list)
    {
        return std::make_shared<GLStream<float>>(
            list);
    }

    IndexStreamPtr GLRenderer::MakeIndexStream(
        std::initializer_list<uint32_t>& list)
    {
        return std::make_shared<GLStream<uint32_t>>(
            list);
    }

    std::shared_ptr<Rendering::InstanceUI> GLRenderer::MakeInstanceUI(
        ContextPtr context,
        VertexStreamPtr vertices,
        IndexStreamPtr indices,
        TextureRGBA8Ptr texture)
    {
        return std::make_shared<GLInstanceUI>(
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
            static_cast<uint32_t>(state.viewport.x),
            static_cast<uint32_t>(state.viewport.y),
            static_cast<uint32_t>(state.viewport.z),
            static_cast<uint32_t>(state.viewport.w));

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

#include "GLRenderer.hpp"

#include <stack>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

#include "GLTexture.hpp"
#include "GLStream.hpp"
#include "GLFrameBuffer.hpp"

#include "../properties/Property.hpp"

using namespace Application::Properties;

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    static std::initializer_list<float> quad_vertices_data
    {
       -1.0f, -1.0f, 0.0f,
       -1.0f, -1.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, -1.0f,
        2.0f, -1.0f, 0.0f,
        2.0f, -1.0f,

       -1.0f,  0.0f, 0.0f,
       -1.0f,  0.0f,
        0.0f,  0.0f, 0.0f,
        0.0f,  0.0f,
        1.0f,  0.0f, 0.0f,
        1.0f,  0.0f,
        2.0f,  0.0f, 0.0f,
        2.0f,  0.0f,

       -1.0f,  1.0f, 0.0f,
       -1.0f,  1.0f,
        0.0f,  1.0f, 0.0f,
        0.0f,  1.0f,
        1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,
        2.0f,  1.0f, 0.0f,
        2.0f,  1.0f,

       -1.0f,  2.0f, 0.0f,
       -1.0f,  2.0f,
        0.0f,  2.0f, 0.0f,
        0.0f,  2.0f,
        1.0f,  2.0f, 0.0f,
        1.0f,  2.0f,
        2.0f,  2.0f, 0.0f,
        2.0f,  2.0f,
    };

    static std::initializer_list<uint32_t> quad_indices_data
    {
         0, 1, 4,
         1, 5, 4,
         1, 2, 5,
         2, 6, 5,
         2, 3, 6,
         3, 7, 6,
         4, 5, 8,
         5, 9, 8,
         5, 6, 9,
         6, 10, 9,
         6, 7, 10,
         7, 11, 10,
         8, 9, 12,
         9, 13, 12,
         9, 10, 13,
         10, 14 ,13,
         10, 11, 14,
         11, 15, 14
    };

    GLRenderer::GLRenderer(
        ContextPtr context,
        std::function<void()> swap_buffers,
        uint32_t width,
        uint32_t height) :
        Renderer(
            context,
            swap_buffers,
            width,
            height),
        context(context)
    {
        GLImgui::Initialise();
        imgui = std::make_unique<GLImgui>(context);

        quad_vertices = std::make_shared<GLStream<float>>(
            StreamUsage::DYNAMIC,
            quad_vertices_data);

        quad_indices = std::make_shared<GLStream<uint32_t>>(
            StreamUsage::DYNAMIC,
            quad_indices_data);

        interface_instance = std::make_unique<GLInterfaceInstance>(
            quad_vertices,
            quad_indices);

        environment_buffer = MakeFrameBufferFloat(
            1024,
            1024);

        environment = MakeTexture("env2.jpg");
    }

    GLRenderer::~GLRenderer()
    {
        GLImgui::Destroy();
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

    TextureRGBA32FPtr GLRenderer::MakeTextureFloat(
        uint32_t width,
        uint32_t height)
    {
        std::shared_ptr<std::vector<float>> data =
            std::make_shared<std::vector<float>>();

        data->resize(
            width * height * 4);

        return std::make_shared<GLTexture<TextureDataFloat>>(
            width,
            height,
            data);
    }

    GLFrameBufferBytePtr GLRenderer::MakeFrameBuffer(
        uint32_t width,
        uint32_t height)
    {
        auto buffer = std::make_shared<GLFrameBuffer<TextureDataByte>>(
            width,
            height);

        return buffer;
    }

    GLFrameBufferFloatPtr GLRenderer::MakeFrameBufferFloat(
        uint32_t width,
        uint32_t height)
    {
        auto buffer = std::make_shared<GLFrameBuffer<TextureDataFloat>>(
            width,
            height);

        return buffer;
    }

    void GLRenderer::Draw(RenderState state)
    {
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glViewport(
            static_cast<uint32_t>(state.viewport.x),
            static_cast<uint32_t>(state.viewport.y),
            static_cast<uint32_t>(state.viewport.z),
            static_cast<uint32_t>(state.viewport.w));

        glClearColor(
            1, 1, 1, 1);

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT);

        glEnable(
            GL_BLEND);

        DrawNodes(state, root_node.get());

        imgui->Draw(state);

        swap_buffers();
    }

    void GLRenderer::DrawNodes(RenderState state, DisplayNode* node)
    {
        interface_instance->Bind(state);

        {
            std::unordered_set<DisplayNode*> visited;
            std::stack<DisplayNode*> stack;
            stack.push(root_node.get());

            while (!stack.empty())
            {
                DisplayNode* s = stack.top();
                stack.pop();

                if (visited.find(s) == visited.end())
                {
                    interface_instance->Draw(state, s, environment);
                    visited.insert(s);
                }

                for (auto c : s->Children())
                {
                    if (visited.find(c) == visited.end())
                    {
                        stack.push(c);
                    }
                }
            }
        }

        interface_instance->Unbind();
    }
}
}
}

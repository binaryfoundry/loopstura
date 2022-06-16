#pragma once

#include "GL.hpp"
#include "GLStream.hpp"
#include "GLFrameBuffer.hpp"
#include "GLImgui.hpp"

#include "../Context.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"

#include <functional>
#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

#include "instances/GLInterfaceInstance.hpp"
#include "instances/GLEnvironmentInstance.hpp"
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

        TextureRGBA32FPtr MakeTextureFloat(
            uint32_t width,
            uint32_t height) override;

    private:
        GLFrameBufferBytePtr MakeFrameBuffer(
            uint32_t width,
            uint32_t height);

        GLFrameBufferFloatPtr MakeFrameBufferFloat(
            uint32_t width,
            uint32_t height);

    protected:
        std::unique_ptr<GLImgui> imgui;

        std::shared_ptr<GLStream<float>> quad_vertices;
        std::shared_ptr<GLStream<uint32_t>> quad_indices;

        std::unique_ptr<GLInterfaceInstance> interface_instance;

        GLFrameBufferFloatPtr environment_buffer;
        std::unique_ptr<GLEnvironmentInstance> environment_instance;

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

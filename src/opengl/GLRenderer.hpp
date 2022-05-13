#pragma once

#include "../Context.hpp"
#include "../rendering/Renderer.hpp"
#include "../rendering/Texture.hpp"

#include "GL.hpp"

#include "rendering/GLInstanceUI.hpp"
#include "rendering/GLInstanceImgui.hpp"

#include <functional>
#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

namespace Application
{
namespace OpenGL
{
    class GLRenderer : public Application::Renderer
    {
    protected:
        ContextPtr context;

        TextureRGBA8Ptr MakeTexture(
            uint32_t width,
            uint32_t height);

        TextureRGBA8Ptr MakeTexture(
            std::string file);

        VertexStreamPtr MakeVertexStream(
            StreamUsage usage,
            std::initializer_list<float>& list);

        IndexStreamPtr MakeIndexStream(
            StreamUsage usage,
            std::initializer_list<uint32_t>& list);

        std::shared_ptr<Rendering::InstanceUI> MakeInstanceUI(
            ContextPtr context,
            VertexStreamPtr vertices,
            IndexStreamPtr indices,
            TextureRGBA8Ptr texture);

        std::shared_ptr<Rendering::InstanceImgui> MakeInstanceImgui(
            ContextPtr context);

    public:
        GLRenderer(
            ContextPtr context,
            std::function<void()> swap_buffers,
            uint32_t width,
            uint32_t height);
        virtual ~GLRenderer();

        void Begin();
        void End();
    };
}
}

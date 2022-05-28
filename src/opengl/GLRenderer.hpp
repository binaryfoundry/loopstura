#pragma once

#include "../Context.hpp"
#include "../rendering/Renderer.hpp"
#include "../rendering/Texture.hpp"

#include "GL.hpp"

#include "GLUserInterface.hpp"
#include "GLImgui.hpp"

#include <functional>
#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

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

        VertexStreamPtr MakeVertexStream(
            StreamUsage usage,
            std::initializer_list<float>& list) override;

        IndexStreamPtr MakeIndexStream(
            StreamUsage usage,
            std::initializer_list<uint32_t>& list) override;

        std::shared_ptr<UserInterface> MakeUserInterface(
            ContextPtr context,
            VertexStreamPtr vertices,
            IndexStreamPtr indices,
            TextureRGBA8Ptr texture) override;

        std::shared_ptr<Imgui> MakeImgui(
            ContextPtr context) override;

    public:
        GLRenderer(
            ContextPtr context,
            std::function<void()> swap_buffers,
            uint32_t width,
            uint32_t height);
        virtual ~GLRenderer();

        void Begin() override;
        void End() override;
    };
}
}
}

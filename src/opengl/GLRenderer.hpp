#pragma once

#include "../Context.hpp"
#include "../rendering/Renderer.hpp"
#include "../rendering/Texture.hpp"

#include "GL.hpp"

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

        std::unique_ptr<GLImgui> imgui;

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

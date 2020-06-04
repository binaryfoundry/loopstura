#pragma once

#include "../Context.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"

#include "GL.hpp"

#include "rendering/GLInstanceUI.hpp"

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

        std::shared_ptr<Rendering::InstanceUI> MakeInstanceUI(
            ContextPtr context,
            std::vector<float>& vertices,
            std::vector<uint32_t>& indices,
            TextureRGBA8Ptr texture);

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

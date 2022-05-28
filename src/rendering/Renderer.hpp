#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "../Context.hpp"
#include "Texture.hpp"
#include "Instance.hpp"

#include "UserInterface.hpp"
#include "Imgui.hpp"

namespace Application
{
namespace Rendering
{
    class Renderer
    {
    protected:
        Renderer(
            ContextPtr context,
            std::function<void()> swap_buffers,
            uint32_t width,
            uint32_t height);

        const ContextPtr context;

        const std::function<void()> swap_buffers;

        uint32_t width = 0;
        uint32_t height = 0;

    public:
        virtual ~Renderer();

        RenderState state;

        void Resize(
            uint32_t width,
            uint32_t height);

        virtual void Begin() = 0;
        virtual void End() = 0;

        virtual TextureRGBA8Ptr MakeTexture(
            uint32_t width,
            uint32_t height) = 0;

        virtual TextureRGBA8Ptr MakeTexture(
            std::string file) = 0;

        virtual VertexStreamPtr MakeVertexStream(
            StreamUsage usage,
            std::initializer_list<float>& list) = 0;

        virtual IndexStreamPtr MakeIndexStream(
            StreamUsage usage,
            std::initializer_list<uint32_t>& list) = 0;

        virtual std::shared_ptr<Rendering::UserInterface> MakeUserInterface(
            ContextPtr context,
            VertexStreamPtr vertices,
            IndexStreamPtr indices,
            TextureRGBA8Ptr texture) = 0;

        virtual std::shared_ptr<Rendering::Imgui> MakeImgui(
            ContextPtr context) = 0;
    };
}
}

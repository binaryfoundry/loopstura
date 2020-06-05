#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "Context.hpp"
#include "Texture.hpp"
#include "rendering/InstanceUI.hpp"

namespace Application
{
    struct RendererState
    {
        glm::mat4 view = mat4();
        glm::mat4 projection = mat4();
        glm::vec4 viewport = vec4();
    };

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

        RendererState state;

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

        virtual std::shared_ptr<Rendering::InstanceUI> MakeInstanceUI(
            ContextPtr context,
            std::vector<float>& vertices,
            std::vector<uint32_t>& indices,
            TextureRGBA8Ptr texture) = 0;
    };
}

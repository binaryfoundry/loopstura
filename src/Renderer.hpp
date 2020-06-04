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

        const std::shared_ptr<Property<glm::mat4>> view;
        const std::shared_ptr<Property<glm::mat4>> projection;
        const std::shared_ptr<Property<glm::mat4>> inverse_projection;
        const std::shared_ptr<Property<glm::mat4>> inverse_view_rotation;
        const std::shared_ptr<Property<glm::vec4>> view_position;
        const std::shared_ptr<Property<glm::vec4>> viewport;

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

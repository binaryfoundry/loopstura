#pragma once

#include "properties/Manager.hpp"

#include <memory>
#include <functional>
#include <string>

namespace Application
{
    struct RenderState
    {
        glm::mat4 view = mat4();
        glm::mat4 projection = mat4();
        glm::vec4 viewport = vec4();
    };

    using Texure2DLoadCallback = std::function<void(
        std::string file,
        uint8_t& pitch,
        uint32_t& width,
        uint32_t& height,
        std::shared_ptr<std::vector<uint8_t>> data)>;

    class Context
    {
        class Client;
        friend class Client;

    public:
        Context(
            Texure2DLoadCallback load_texture_2d);

    public:
        ~Context();

        const std::unique_ptr<Properties::Manager> property_manager;

        const Texure2DLoadCallback load_texture_2d;

        void Update();
    };

    using ContextPtr = std::shared_ptr<Context>;
}

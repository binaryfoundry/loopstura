#pragma once

#include <memory>
#include <string>

#include "Context.hpp"
#include "Renderer.hpp"
#include "Stream.hpp"

namespace Application
{
    class Client
    {
    private:
        const std::shared_ptr<Context> context;
        const std::shared_ptr<Renderer> renderer;

        std::shared_ptr<Rendering::InstanceUI> quad_instance;
        TextureRGBA8Ptr quad_texture;

        VertexStreamPtr quad_vertices;
        IndexStreamPtr quad_indices;

        TextureRGBA8Ptr imgui_texture;

    public:
        Client(
            ContextPtr context,
            std::shared_ptr<Renderer> renderer);
        virtual ~Client();

        void Update();
        void Render();
    };
}

#pragma once

#include <memory>
#include <string>

#include "Context.hpp"

#include "rendering/Renderer.hpp"
#include "rendering/Stream.hpp"
#include "rendering/InstanceUI.hpp"
#include "rendering/InstanceImgui.hpp"

#include "sdl/SDLAudio.hpp"

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

        std::shared_ptr<Rendering::InstanceImgui> imgui_instance;

        SDLAudio audio;

    public:
        Client(
            ContextPtr context,
            std::shared_ptr<Renderer> renderer);
        virtual ~Client();

        void Update();
        void Render();
    };
}

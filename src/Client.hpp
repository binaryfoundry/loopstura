#pragma once

#include <memory>
#include <string>

#include "Context.hpp"

#include "rendering/Renderer.hpp"

#include "audio/Track.hpp"

using namespace Application::Rendering;

namespace Application
{
    class Client
    {
    private:
        const std::shared_ptr<Context> context;
        const std::shared_ptr<Renderer> renderer;

        std::unique_ptr<Track> track;

        bool dock_created = false;
        void DrawDock(bool* p_open);
        void DrawPlot();

    public:
        Client(
            ContextPtr context,
            std::shared_ptr<Renderer> renderer);
        virtual ~Client();

        void Update();
        void Render();
    };
}

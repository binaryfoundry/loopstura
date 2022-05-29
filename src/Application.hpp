#pragma once

#include <memory>
#include <string>

#include "Context.hpp"
#include "DisplayNode.hpp"
#include "Renderer.hpp"

#include "audio/Track.hpp"

using namespace Application::Rendering;

namespace Application
{
    class Application
    {
    private:
        const std::shared_ptr<Context> context;
        const std::shared_ptr<Renderer> renderer;

        std::unique_ptr<Track> track;

        bool dock_created = false;
        void DrawDock(bool* p_open);
        void DrawPlot();

        DisplayNodePtr display_root;
        DisplayNodePtr background_quad;

    public:
        Application(
            ContextPtr context,
            std::shared_ptr<Renderer> renderer);
        virtual ~Application();

        void Update();
        void Render();
    };
}

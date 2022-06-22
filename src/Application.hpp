#pragma once

#include <memory>
#include <string>

#include "Context.hpp"
#include "Component.hpp"
#include "DisplayNode.hpp"
#include "Renderer.hpp"
#include "Timer.hpp"

#include "audio/Track.hpp"

using namespace Application::Rendering;

namespace Application
{
    class Application
    {
    private:
        const std::shared_ptr<Context> context;
        const std::shared_ptr<Renderer> renderer;

        bool dock_created = false;
        void DrawDock(bool* p_open);

        DisplayNodePtr display_root;

        ComponentPtr test_button;
        ComponentPtr test_button_2;

        std::shared_ptr<Track> track;
        ComponentPtr waveform0;

        Timer fps_timer;

    public:
        Application(
            ContextPtr context,
            std::shared_ptr<Renderer> renderer);
        virtual ~Application();

        void Update();
        void Render();
    };
}

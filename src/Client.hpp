#pragma once

#include <memory>

#include "Renderer.hpp"

namespace Application
{
    class Client
    {
    private:
        const std::shared_ptr<Renderer> renderer;

    public:
        Client(std::shared_ptr<Renderer> renderer);
        virtual ~Client();

        void Update();
        void Render();
    };
}

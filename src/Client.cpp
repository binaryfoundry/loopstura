#include "Client.hpp"

namespace Application
{
    Client::Client(
        std::shared_ptr<Renderer> renderer) :
        renderer(renderer)
    {
    }

    Client::~Client()
    {
    }

    void Client::Update()
    {
    }

    void Client::Render()
    {
        renderer->Render();
    }
}

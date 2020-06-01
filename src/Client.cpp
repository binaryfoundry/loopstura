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
        auto& texture_data = renderer->texture_data;

        for (size_t i = 0; i < texture_data.size(); i++)
        {
            texture_data[i] = rand() % 255;
        }
    }

    void Client::Render()
    {
        renderer->Render();
    }
}

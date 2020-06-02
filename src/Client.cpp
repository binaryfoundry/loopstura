#include "Client.hpp"

namespace Application
{
    Client::Client(
        std::shared_ptr<Renderer> renderer) :
        renderer(renderer),
        context(std::make_shared<Context>())
    {
    }

    Client::~Client()
    {
    }

    void Client::Update()
    {
        context->Update();

        auto texture_data = renderer->texture->data;

        for (size_t i = 0; i < texture_data->size(); i++)
        {
            (*texture_data)[i] = rand() % 255;
        }

        renderer->texture->Invalidate();
    }

    void Client::Render()
    {
        renderer->Render();
    }
}

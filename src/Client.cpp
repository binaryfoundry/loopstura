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

        uint8_t v = 0;

        for (size_t i = 0; i < texture_data->size(); i+=4)
        {
            if (i % 16 == 0) v = rand() % 255;
            (*texture_data)[i + 0] = v;
            (*texture_data)[i + 1] = v;
            (*texture_data)[i + 2] = v;
            (*texture_data)[i + 3] = 255;
        }

        renderer->texture->Invalidate();
    }

    void Client::Render()
    {
        renderer->Render();
    }
}

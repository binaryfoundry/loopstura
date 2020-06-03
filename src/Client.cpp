#include "Client.hpp"

namespace Application
{
    Client::Client(
        ContextPtr context,
        std::shared_ptr<Renderer> renderer) :
        renderer(renderer),
        context(context)
    {
    }

    Client::~Client()
    {
    }

    void Client::Update()
    {
        context->Update();

        auto quad_texture = renderer->quad_instance->texture;
        auto quad_texture_data = quad_texture->data;

        uint8_t v = 0;

        for (size_t i = 0; i < quad_texture_data->size(); i+=4)
        {
            if (i % 16 == 0) v = rand() % 255;
            (*quad_texture_data)[i + 0] = v;
            (*quad_texture_data)[i + 1] = v;
            (*quad_texture_data)[i + 2] = v;
            (*quad_texture_data)[i + 3] = 255;
        }

        quad_texture->Invalidate();
    }

    void Client::Render()
    {
        renderer->Draw();
    }
}

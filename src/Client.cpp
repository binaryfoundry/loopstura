#include "Client.hpp"

namespace Application
{
    static std::vector<float> quad_vertices =
    {
        -1.0f,  1.0f, 0.0f,
         1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f,
         1.0f,  1.0f, 0.0f,
         0.0f,  0.0f
    };

    static std::vector<uint32_t> quad_indices =
    {
         0, 1, 2, 2, 3, 0
    };

    Client::Client(
        ContextPtr context,
        std::shared_ptr<Renderer> renderer) :
        renderer(renderer),
        context(context)
    {
        quad_texture = renderer->MakeTexture(
            512,
            512);

        quad_instance = renderer->MakeInstanceBasic(
            context,
            quad_vertices,
            quad_indices,
            quad_texture);
    }

    Client::~Client()
    {
    }

    void Client::Update()
    {
        context->Update();

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
        renderer->Begin();
        quad_instance->Draw();
        renderer->End();
    }
}

#include "Client.hpp"

namespace Application
{
    static std::initializer_list<float> quad_vertices_data
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

    static std::initializer_list<uint32_t> quad_indices_data
    {
         0, 1, 2, 2, 3, 0
    };

    Client::Client(
        ContextPtr context,
        std::shared_ptr<Renderer> renderer) :
        renderer(renderer),
        context(context)
    {
        quad_vertices = std::make_shared<VertexStream>(
            quad_vertices_data);

        quad_indices = std::make_shared<IndexStream>(
            quad_indices_data);

        quad_texture = renderer->MakeTexture(
            "test.png");

        quad_instance = renderer->MakeInstanceUI(
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
            (*quad_texture_data)[i + 3] = 255;
        }

        quad_texture->Invalidate();
    }

    void Client::Render()
    {
        RenderState state = renderer->state;

        renderer->Begin();

        state.view = glm::scale(
            state.view,
            glm::vec3(100.0f));

        quad_instance->Draw(state);
        renderer->End();
    }
}

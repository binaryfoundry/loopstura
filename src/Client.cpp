#include "Client.hpp"

namespace Application
{
    static std::initializer_list<float> quad_vertices_data
    {
        -1.0f,  1.0f, 0.0f,
         0.0f,  1.0f,
        -1.0f, -1.0f, 0.0f,
         0.0f,  0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f,  1.0f
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
        quad_vertices = renderer->MakeVertexStream(
            StreamUsage::DYNAMIC,
            quad_vertices_data);

        quad_indices = renderer->MakeIndexStream(
            StreamUsage::DYNAMIC,
            quad_indices_data);

        quad_texture = renderer->MakeTexture(
            "test.png");

        quad_instance = renderer->MakeInstanceUI(
            context,
            quad_vertices,
            quad_indices,
            quad_texture);

        quad_instance->brightness->Set(
            1.0);

        quad_instance->gradient->Set(
            1.0);

        quad_instance->gradient_0->Set(
            vec3(0.36, 0.65, 0.74));

        quad_instance->gradient_1->Set(
            vec3(0.16, 0.27, 0.63));

        context->property_manager->AddTween(
            quad_instance->brightness,
            0.0f,
            1.0f,
            EasingFunction::EaseOutCubic);
    }

    Client::~Client()
    {
    }

    void Client::Update()
    {
        context->Update();
    }

    void Client::Render()
    {
        RenderState state = renderer->state;

        renderer->Begin();

        vec3 scale = glm::vec3(
            renderer->state.viewport.z,
            renderer->state.viewport.w,
            1.0);

        state.view = glm::scale(
            state.view,
            scale);

        quad_instance->Draw(state);
        renderer->End();
    }
}

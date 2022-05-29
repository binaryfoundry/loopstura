#include "Quad.hpp"

namespace Application
{
    Quad::Quad()
    {
        Init();

        passthrough = true;
    }

    Quad::Quad(
        ContextPtr context,
        std::shared_ptr<Quad> parent,
        TextureRGBA8Ptr texture) :
        context(context),
        parent(parent),
        texture(texture)
    {
        Init();

        brightness = std::make_shared<Property<float>>(0.0f);
        gradient = std::make_shared<Property<float>>(0.0f);
        gradient_0 = std::make_shared<Property<vec3>>(vec3());
        gradient_1 = std::make_shared<Property<vec3>>(vec3());

        brightness->Set(
            1.0);

        gradient->Set(
            1.0);

        gradient_0->Set(
            vec3(0.36, 0.65, 0.74));

        gradient_1->Set(
            vec3(0.16, 0.27, 0.63));
    }

    void Quad::Init()
    {
        position = std::make_shared<Property<vec2>>(
            vec2(),
            &dirty_flag);

        scale = std::make_shared<Property<vec2>>(
            vec2(1.0, 1.0),
            &dirty_flag);
    }

    void Quad::Validate()
    {
        if (parent != nullptr)
        {
            parent->Validate();
        }

        if (!dirty_flag)
        {
            return;
        }

        transform = mat4();

        if (parent != nullptr)
        {
            transform = parent->transform;
        }

        transform = glm::translate(
            transform,
            vec3(position->Value(), 1.0f));

        transform = glm::scale(
            transform,
            vec3(scale->Value(), 1.0f));

        dirty_flag = false;
    }
}

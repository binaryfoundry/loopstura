#include "Quad.hpp"

namespace Application
{
    Quad::Quad(
        ContextPtr context,
        TextureRGBA8Ptr texture) :
        context(context),
        texture(texture)
    {
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

        context->property_manager->AddTween(
            brightness,
            0.0f,
            1.0f,
            EasingFunction::EaseOutCubic);
    }
}

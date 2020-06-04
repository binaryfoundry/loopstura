#include "InstanceUI.hpp"

namespace Application
{
namespace Rendering
{
    InstanceUI::InstanceUI(
        ContextPtr context,
        std::vector<float>& vertices,
        std::vector<uint32_t>& indices,
        TextureRGBA8Ptr texture) :
        vertices(vertices),
        indices(indices),
        texture(texture),
        fade(std::make_shared<Property<float>>(0.0f))
    {
        context->property_manager->AddTween(
            fade, 1.0f, 1.0f, EasingFunction::EaseOutCubic);
    }

    InstanceUI::~InstanceUI()
    {
    }
}
}

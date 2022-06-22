#include "ButtonComponent.hpp"

namespace Application
{
namespace Components
{
    ButtonComponent::ButtonComponent(
        ContextPtr context,
        glm::vec2 position,
        glm::vec2 scale,
        DisplayNodeSDFFunction sdf_func,
        std::shared_ptr<DisplayNode> parent) :
        Component(parent)
    {
        *root->position = position;

        button = std::make_shared<DisplayNode>(
            context,
            root);

        *button->scale = scale;
        *button->brightness = 0.0;
        *button->alpha_margin = 1.0;
        *button->gradient_0 = vec3(1.0, 0.843, 0.0) * 0.75f;
        *button->gradient_1 = vec3(1.0, 0.843, 0.0);
        *button->roughness = 0.0;
        button->sdf_func = sdf_func;
    }

    void ButtonComponent::Update(RenderState state)
    {
    }
}
}

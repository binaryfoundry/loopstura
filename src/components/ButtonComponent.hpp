#pragma once

#include "../Component.hpp"

namespace Application
{
namespace Components
{
    class ButtonComponent : public Component
    {
    private:
        DisplayNodePtr button;

    public:
        ButtonComponent(
            ContextPtr context,
            glm::vec2 position,
            glm::vec2 scale,
            DisplayNodeSDFFunction sdf_func,
            std::shared_ptr<DisplayNode> parent);

        void Update(RenderState state) override;
    };
}
}

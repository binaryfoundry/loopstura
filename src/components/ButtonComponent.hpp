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
            DisplayNodeSDF sdf_func,
            DisplayNodePtr parent);

        void Update(RenderState state) override;
    };
}
}

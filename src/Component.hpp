#pragma once

#include <memory>

#include "Context.hpp"
#include "DisplayNode.hpp"

namespace Application
{
    class Component
    {
    protected:
        DisplayNodePtr root;

        Component(DisplayNodePtr parent)
        {
            root = std::make_shared<DisplayNode>(parent);
        }

    public:
        DisplayNodePtr Root()
        {
            return root;
        }

        virtual void Update(RenderState state) = 0;
    };

    using ComponentPtr = std::shared_ptr<Component>;
}

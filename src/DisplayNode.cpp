#include "DisplayNode.hpp"

namespace Application
{
    DisplayNode::DisplayNode(
        std::shared_ptr<DisplayNode> parent) :
        parent(parent)
    {
        Init();

        passthrough = true;
    }

    DisplayNode::DisplayNode(
        ContextPtr context,
        std::shared_ptr<DisplayNode> parent,
        TexturePtr texture) :
        context(context),
        parent(parent),
        texture(texture)
    {
        Init();

        texture_blend = std::make_shared<Property<float>>(0.0f);
        brightness = std::make_shared<Property<float>>(0.0f);
        gradient = std::make_shared<Property<float>>(0.0f);
        alpha_margin = std::make_shared<Property<float>>(0.0f);
        nonlinearity = std::make_shared<Property<float>>(8.0f);
        metalness = std::make_shared<Property<float>>(0.0f);
        roughness = std::make_shared<Property<float>>(1.0f);
        gradient_0 = std::make_shared<Property<vec3>>(vec3(0.0, 0.0, 0.0));
        gradient_1 = std::make_shared<Property<vec3>>(vec3(1.0, 1.0, 1.0));
    }

    void DisplayNode::Init()
    {
        if (parent != nullptr)
        {
            parent->children.push_back(this);
        }

        auto invalidate = [&]() {
            Invalidate();
        };

        position = std::make_shared<Property<vec2>>(
            vec2(),
            invalidate);

        scale = std::make_shared<Property<vec2>>(
            vec2(1.0, 1.0),
            invalidate);
    }

    void DisplayNode::Invalidate()
    {
        dirty_flag = true;

        for (auto& child : children)
        {
            if (!child->dirty_flag)
            {
                child->Invalidate();
            }
        }
    }

    void DisplayNode::Validate()
    {
        if (!dirty_flag)
        {
            return;
        }

        if (parent != nullptr)
        {
            parent->Validate();
        }

        transform = mat4();

        if (parent != nullptr)
        {
            transform = parent->transform;
        }

        transform = glm::scale(
            transform,
            vec3(scale->Value(), 1.0f));

        transform = glm::translate(
            transform,
            vec3(position->Value(), 0.0f));

        dirty_flag = false;
    }
}

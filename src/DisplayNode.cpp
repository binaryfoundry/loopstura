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
        TextureRGBA8Ptr texture) :
        context(context),
        parent(parent),
        texture(texture)
    {
        Init();

        texture_blend = std::make_shared<Property<float>>(0.0f);
        outline_margin = std::make_shared<Property<float>>(0.0f);
        brightness = std::make_shared<Property<float>>(0.0f);
        gradient = std::make_shared<Property<float>>(0.0f);
        alpha_margin = std::make_shared<Property<float>>(0.0f);
        gradient_0 = std::make_shared<Property<vec3>>(vec3(0.0, 0.0, 0.0));
        gradient_1 = std::make_shared<Property<vec3>>(vec3(1.0, 1.0, 1.0));
    }

    void DisplayNode::Init()
    {
        if (parent != nullptr)
        {
            parent->children.push_back(this);
        }

        position = std::make_shared<Property<vec2>>(
            vec2(),
            &dirty_flag);

        scale = std::make_shared<Property<vec2>>(
            vec2(1.0, 1.0),
            &dirty_flag);
    }

    void DisplayNode::Validate()
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

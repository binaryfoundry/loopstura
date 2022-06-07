#pragma once

#include <memory>
#include "properties/Property.hpp"

#include "Context.hpp"
#include "Texture.hpp"

using namespace Application::Properties;
using namespace Application::Rendering;

namespace Application
{
    enum class DisplayNodeSDFFunction
    {
        NONE,
        CIRCLE,
        BOX_ROUNDED,
        CYLINDER_HORIZONTAL,
        WAVEFORM
    };

    class DisplayNode
    {
    private:
        ContextPtr context;

        bool dirty_flag = true;
        bool passthrough = false;

        glm::mat4 transform;

        TexturePtr texture = nullptr;

        std::shared_ptr<DisplayNode> parent = nullptr;

        void Init();

        std::vector<DisplayNode*> children;

    public:
        std::shared_ptr<Property<glm::vec2>> scale;
        std::shared_ptr<Property<glm::vec2>> position;

        std::shared_ptr<Property<float>> texture_blend;
        std::shared_ptr<Property<float>> brightness;
        std::shared_ptr<Property<float>> gradient;
        std::shared_ptr<Property<float>> alpha_margin;
        std::shared_ptr<Property<float>> nonlinearity;
        std::shared_ptr<Property<float>> metalness;
        std::shared_ptr<Property<float>> roughness;
        std::shared_ptr<Property<vec3>> gradient_0;
        std::shared_ptr<Property<vec3>> gradient_1;

        glm::vec2 tex_scale = glm::vec2(1.0f, 1.0f);
        bool maintain_pixel_scaling_horizontal = false;

        DisplayNodeSDFFunction sdf_func = DisplayNodeSDFFunction::NONE;

        DisplayNode(
            std::shared_ptr<DisplayNode> parent = nullptr);

        DisplayNode(
            ContextPtr context,
            std::shared_ptr<DisplayNode> parent = nullptr,
            TexturePtr texture = nullptr);

        glm::mat4 Transform()
        {
            return transform;
        }

        TexturePtr Texture()
        {
            return texture;
        }

        bool Passthrough() const
        {
            return passthrough;
        }

        std::vector<DisplayNode*>& Children()
        {
            return children;
        }

        void Validate();
    };

    using DisplayNodePtr = std::shared_ptr<DisplayNode>;
}

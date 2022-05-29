#pragma once

#include <memory>
#include "properties/Property.hpp"

#include "Context.hpp"
#include "Texture.hpp"

using namespace Application::Properties;
using namespace Application::Rendering;

namespace Application
{
    class Quad
    {
    private:
        ContextPtr context;

        bool dirty_flag = true;

        glm::mat4 transform;

        TextureRGBA8Ptr texture = nullptr;

    public:
        std::shared_ptr<Property<glm::vec2>> scale;
        std::shared_ptr<Property<glm::vec2>> position;

        std::shared_ptr<Property<float>> brightness;
        std::shared_ptr<Property<float>> gradient;
        std::shared_ptr<Property<vec3>> gradient_0;
        std::shared_ptr<Property<vec3>> gradient_1;

        Quad(ContextPtr context, TextureRGBA8Ptr texture = nullptr);

        glm::mat4 Transform()
        {
            return transform;
        }

        TextureRGBA8Ptr Texture()
        {
            return texture;
        }

        void Validate();
    };

    using QuadPtr = std::shared_ptr<Quad>;
}

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

    public:
        glm::mat3 transform;

        TextureRGBA8Ptr texture;

        std::shared_ptr<Property<float>> brightness;
        std::shared_ptr<Property<float>> gradient;
        std::shared_ptr<Property<vec3>> gradient_0;
        std::shared_ptr<Property<vec3>> gradient_1;

        Quad(ContextPtr context, TextureRGBA8Ptr texture);
    };

    using QuadPtr = std::shared_ptr<Quad>;
}

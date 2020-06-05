#pragma once

#include <memory>

#include "../Instance.hpp"
#include "../Texture.hpp"
#include "../Context.hpp"

#include "../properties/Property.hpp"

using namespace Application::Properties;

namespace Application
{
namespace Rendering
{
    class InstanceUI : public Instance
    {
    protected:
        InstanceUI(
            ContextPtr context,
            VerticesPtr vertices,
            IndicesPtr indices,
            TextureRGBA8Ptr texture);

    public:
        const VerticesPtr vertices;
        const IndicesPtr indices;

        const TextureRGBA8Ptr texture;

        std::shared_ptr<Property<float>> fade;

        virtual ~InstanceUI();
    };
}
}

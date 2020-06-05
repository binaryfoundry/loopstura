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
            VertexStreamPtr vertices,
            IndexStreamPtr indices,
            TextureRGBA8Ptr texture);

    public:
        const VertexStreamPtr vertices;
        const IndexStreamPtr indices;

        const TextureRGBA8Ptr texture;

        std::shared_ptr<Property<float>> fade;

        virtual ~InstanceUI();
    };
}
}

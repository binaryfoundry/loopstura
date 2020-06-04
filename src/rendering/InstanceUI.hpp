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
            std::vector<float>& vertices,
            std::vector<uint32_t>& indices,
            TextureRGBA8Ptr texture);

    public:
        const std::vector<float>& vertices;
        const std::vector<uint32_t>& indices;

        const TextureRGBA8Ptr texture;

        std::shared_ptr<Property<float>> fade;

        virtual ~InstanceUI();
    };
}
}

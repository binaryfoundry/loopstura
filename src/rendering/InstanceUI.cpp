#include "InstanceUI.hpp"

namespace Application
{
namespace Rendering
{
    InstanceUI::InstanceUI(
        ContextPtr context,
        VertexStreamPtr vertices,
        IndexStreamPtr indices,
        TextureRGBA8Ptr texture) :
        vertices(vertices),
        indices(indices),
        texture(texture),
        brightness(std::make_shared<Property<float>>(0.0f)),
        gradient(std::make_shared<Property<float>>(0.0f)),
        gradient_0(std::make_shared<Property<vec3>>(vec3())),
        gradient_1(std::make_shared<Property<vec3>>(vec3()))
    {
    }

    InstanceUI::~InstanceUI()
    {
    }
}
}

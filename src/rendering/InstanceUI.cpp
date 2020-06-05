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
        brightness(std::make_shared<Property<float>>(0.0f))
    {
    }

    InstanceUI::~InstanceUI()
    {
    }
}
}

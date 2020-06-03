#include "InstanceBasic.hpp"

namespace Application
{
namespace Rendering
{
    InstanceBasic::InstanceBasic(
        ContextPtr context,
        std::vector<float>& vertices,
        std::vector<uint32_t>& indices,
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture) :
        vertices(vertices),
        indices(indices),
        texture(texture)
    {
    }

    InstanceBasic::~InstanceBasic()
    {
    }
}
}

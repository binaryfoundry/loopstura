#include "InstanceBasic.hpp"

namespace Application
{
namespace Rendering
{
    InstanceBasic::InstanceBasic(
        ContextPtr context,
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture) :
        texture(texture)
    {
    }

    InstanceBasic::~InstanceBasic()
    {
    }
}
}

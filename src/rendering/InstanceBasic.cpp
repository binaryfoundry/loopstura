#include "InstanceBasic.hpp"

namespace Application
{
namespace Rendering
{
    InstanceBasic::InstanceBasic(
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture) :
        texture(texture)
    {
    }

    InstanceBasic::~InstanceBasic()
    {
    }
}
}

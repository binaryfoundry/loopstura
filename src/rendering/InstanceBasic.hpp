#pragma once

#include <memory>

#include "../Instance.hpp"
#include "../Texture.hpp"

namespace Application
{
namespace Rendering
{
    class InstanceBasic : public Instance
    {
    protected:
        InstanceBasic(
            std::shared_ptr<Texture<TextureDataByteRGBA>> texture);

    public:
        const std::shared_ptr<Texture<TextureDataByteRGBA>> texture;

        virtual ~InstanceBasic();
    };
}
}

#pragma once

#include <memory>

#include "../Instance.hpp"
#include "../Texture.hpp"
#include "../Context.hpp"

namespace Application
{
namespace Rendering
{
    class InstanceBasic : public Instance
    {
    protected:
        InstanceBasic(
            ContextPtr context,
            std::shared_ptr<Texture<TextureDataByteRGBA>> texture);

    public:
        const std::shared_ptr<Texture<TextureDataByteRGBA>> texture;

        virtual ~InstanceBasic();
    };
}
}

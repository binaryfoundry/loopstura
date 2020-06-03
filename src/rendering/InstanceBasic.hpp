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
            std::vector<float>& vertices,
            std::vector<uint32_t>& indices,
            std::shared_ptr<Texture<TextureDataByteRGBA>> texture);

    public:
        const std::vector<float>& vertices;
        const std::vector<uint32_t>& indices;

        const std::shared_ptr<Texture<TextureDataByteRGBA>> texture;

        virtual ~InstanceBasic();
    };
}
}
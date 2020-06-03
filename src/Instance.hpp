#pragma once

#include "Context.hpp"

namespace Application
{
    class Instance
    {
    protected:
        Instance(
            std::vector<float>& vertices,
            std::vector<uint32_t>& indices) :
            vertices(vertices),
            indices(indices)
        {
        }

        virtual ~Instance()
        {
        }

        const ContextPtr context;

        const std::vector<float>& vertices;
        const std::vector<uint32_t>& indices;

    public:
        virtual void Draw() = 0;
    };
}

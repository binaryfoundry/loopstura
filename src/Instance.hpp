#pragma once

#include "Context.hpp"

namespace Application
{
    using VerticesPtr = std::shared_ptr<std::vector<float>>;
    using IndicesPtr = std::shared_ptr<std::vector<uint32_t>>;

    class Instance
    {
    protected:
        const ContextPtr context;

    public:
        virtual void Draw() = 0;
    };
}

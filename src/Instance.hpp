#pragma once

#include "Context.hpp"

namespace Application
{
    class Instance
    {
    protected:
        const ContextPtr context;

    public:
        virtual void Draw() = 0;
    };
}

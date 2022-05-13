#pragma once

#include "../Context.hpp"
#include "Stream.hpp"

namespace Application
{

    struct RenderState
    {
        glm::mat4 view = mat4();
        glm::mat4 projection = mat4();
        glm::vec4 viewport = vec4();
    };

    class Instance
    {
    protected:
        const ContextPtr context;

    public:
        virtual void Draw(RenderState state) = 0;
    };
}

#pragma once

#include "../Context.hpp"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    class GLInstance
    {
        virtual void Bind(RenderState state) = 0;
        virtual void Unbind() = 0;
    };
}
}
}

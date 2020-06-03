#pragma once

#include "GL.hpp"

#include <string>
#include <vector>

namespace Application
{
namespace OpenGL
{
    class GLInstance
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        std::vector<float>& vertices;
        std::vector<uint32_t>& indices;

        GLuint gl_vertex_buffer = 0;
        GLuint gl_index_buffer = 0;

    public:
        GLInstance(
            std::vector<float>& vertices,
            std::vector<uint32_t>& indices);
        virtual ~GLInstance();

        virtual void Draw() = 0;
    };
}
}

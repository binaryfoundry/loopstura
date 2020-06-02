#pragma once

#include "GL.hpp"

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
namespace OpenGL
{
    class GLMesh
    {
        class GLRenderer;
        friend class GLRenderer;

    private:
        GLuint quad_vertex_buffer = 0;
        GLuint quad_index_buffer = 0;

    public:
        GLMesh();
        virtual ~GLMesh();

        void Draw();
    };
}
}

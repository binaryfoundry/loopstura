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
        GLuint& shader_program;

        std::vector<GLfloat>& vertices;
        std::vector<GLuint>& indices;

        GLuint gl_vertex_buffer = 0;
        GLuint gl_index_buffer = 0;

    public:
        GLInstance(
            GLuint& shader_program,
            std::vector<GLfloat>& vertices,
            std::vector<GLuint>& indices);
        virtual ~GLInstance();

        virtual void Draw() = 0;
    };
}
}

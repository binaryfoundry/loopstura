#pragma once

#include "GL.hpp"

#include <string>

namespace Application
{
namespace OpenGL
{
    GLuint LinkShader(
        std::string vertex_shader_string,
        std::string fragment_shader_string);

    GLuint LoadShader(
        GLenum type,
        const char *shader_src);

    class GLMaterial : public GLBindable
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        GLuint shader = 0;
    };
}
}

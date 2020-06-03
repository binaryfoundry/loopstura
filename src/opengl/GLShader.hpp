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
}
}

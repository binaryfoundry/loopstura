#pragma once

#include "GL.hpp"

#include <string>
#include <vector>

namespace Application
{
namespace OpenGL
{
    template<typename T>
    static GLuint GenBuffer(
        std::vector<T>& data,
        GLenum target,
        GLenum usage)
    {
        GLuint buffer;

        glGenBuffers(
            1,
            &buffer);

        glBindBuffer(
            target,
            buffer);

        glBufferData(
            target,
            sizeof(T) * data.size(),
            &data[0],
            usage);

        glBindBuffer(
            target,
            NULL);

        return buffer;
    }

    GLuint LinkShader(
        std::string vertex_shader_string,
        std::string fragment_shader_string);
}
}

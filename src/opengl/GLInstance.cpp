#include "GLInstance.hpp"

#include <string>
#include <iostream>

namespace Application
{
namespace OpenGL
{
    template<typename T>
    GLuint GenBuffer(
        std::vector<T>& data,
        GLenum target,
        GLenum usage);

    GLInstance::GLInstance(
        GLuint& shader_program,
        std::vector<GLfloat>& vertices,
        std::vector<GLuint>& indices) :
        shader_program(shader_program),
        vertices(vertices),
        indices(indices)
    {
        gl_vertex_buffer = GenBuffer(
            vertices,
            GL_ARRAY_BUFFER,
            GL_STATIC_DRAW);

        gl_index_buffer = GenBuffer(
            indices,
            GL_ELEMENT_ARRAY_BUFFER,
            GL_STATIC_DRAW);
    }

    GLInstance::~GLInstance()
    {
        glDeleteBuffers(
            1, &gl_vertex_buffer);

        glDeleteBuffers(
            1, &gl_index_buffer);
    }

    template<typename T>
    GLuint GenBuffer(
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
}
}

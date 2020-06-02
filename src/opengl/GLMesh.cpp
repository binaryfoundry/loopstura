#include "GLMesh.hpp"

namespace Application
{
namespace OpenGL
{
    std::vector<GLfloat> quad_vertices =
    {
        -1.0f,  1.0f, 0.0f,
         1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f,
         1.0f,  1.0f, 0.0f,
         0.0f,  0.0f
    };

    std::vector<GLuint> quad_indices =
    {
         0, 1, 2, 2, 3, 0
    };

    template<typename T>
    GLuint GenBuffer(
        std::vector<T>& data,
        GLenum target,
        GLenum usage);

    GLMesh::GLMesh()
    {
        quad_vertex_buffer = GenBuffer(
            quad_vertices,
            GL_ARRAY_BUFFER,
            GL_STATIC_DRAW);

        quad_index_buffer = GenBuffer(
            quad_indices,
            GL_ELEMENT_ARRAY_BUFFER,
            GL_STATIC_DRAW);
    }

    GLMesh::~GLMesh()
    {
        glDeleteBuffers(
            1, &quad_vertex_buffer);

        glDeleteBuffers(
            1, &quad_index_buffer);
    }

    void GLMesh::Draw()
    {
        glBindBuffer(
            GL_ARRAY_BUFFER,
            quad_vertex_buffer);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(GLfloat),
            (GLvoid*)0);

        glEnableVertexAttribArray(1);

        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(GLfloat),
            (GLvoid*)(3 * sizeof(GLfloat)));

        glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER,
            quad_index_buffer);

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(quad_indices.size()),
            GL_UNSIGNED_INT,
            static_cast<char const*>(0));
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

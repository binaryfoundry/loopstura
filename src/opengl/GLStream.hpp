#pragma once

#include "GL.hpp"

#include "../Stream.hpp"

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
namespace OpenGL
{
    class GLBufferHandle
    {
    public:
        GLuint gl_buffer_handle = 0; // TODO make private
    };

    template <typename T>
    class GLStream : public Stream<T>, public GLBufferHandle
    {
    protected:
        void Initialise();

        GLenum target = GL_ARRAY_BUFFER;
        GLenum usage = GL_STATIC_DRAW;

    public:
        GLStream(
            std::initializer_list<T>& list);
        virtual ~GLStream();

        void Update();
    };
}
}

#pragma once

#include "GL.hpp"

#include "../rendering/Stream.hpp"

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
        void Initialise(
            StreamUsage usage);

        const GLenum gl_target = GL_ARRAY_BUFFER;
        GLenum gl_usage = GL_STATIC_DRAW;

    public:
        GLStream(
            StreamUsage usage,
            std::initializer_list<T>& list);
        virtual ~GLStream();

        void Update() override;
    };
}
}

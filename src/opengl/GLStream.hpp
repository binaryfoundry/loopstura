#pragma once

#include "GL.hpp"

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    enum class StreamUsage
    {
        STATIC,
        DYNAMIC
    };

    class GLBufferHandle
    {
    public:
        GLuint gl_buffer_handle = 0; // TODO make private
    };

    template <typename T>
    class GLStream : public GLBufferHandle
    {
    protected:
        void Initialise(
            StreamUsage usage);

        const GLenum gl_target = GL_ARRAY_BUFFER;
        GLenum gl_usage = GL_STATIC_DRAW;

        bool invalidated = true;

        void Invalidate()
        {
            invalidated = true;
        }

    public:
        GLStream(
            StreamUsage usage,
            std::initializer_list<T>& list);
        virtual ~GLStream();

        void Update();

        const std::unique_ptr<std::vector<T>> data;
    };
}
}
}

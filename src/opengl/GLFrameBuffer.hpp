#pragma once

#include "GL.hpp"
#include "../Texture.hpp"

#include <memory>

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    template <typename T>
    class GLFrameBuffer : public TextureTyped<T>, public GLTextureHandle
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        GLint gl_internal_format = 0;
        GLenum gl_format = 0;
        GLenum gl_type = 0;

        GLuint gl_frame_buffer = 0;
        GLuint gl_depth_renderbuffer = 0;

        void Create();

    public:
        GLFrameBuffer(
            uint32_t width,
            uint32_t height);
        virtual ~GLFrameBuffer();

        void Bind();
        void Unbind();
        void GenerateMipMaps();

        void Update() override;
    };

    using GLFrameBufferBytePtr = std::shared_ptr<GLFrameBuffer<TextureDataByte>>;
    using GLFrameBufferFloatPtr = std::shared_ptr<GLFrameBuffer<TextureDataFloat>>;
}
}
}

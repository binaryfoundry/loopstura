#include "GLFrameBuffer.hpp"

#include <iostream>
#include <cassert>

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    template <>
    GLFrameBuffer<TextureDataByte>::GLFrameBuffer(
        uint32_t width,
        uint32_t height) :
        gl_internal_format(GL_RGBA8),
        gl_format(GL_RGBA),
        gl_type(GL_UNSIGNED_BYTE),
        TextureTyped<TextureDataByte>(
            width,
            height,
            nullptr)
    {
        Create();
    }

    template <>
    GLFrameBuffer<TextureDataFloat>::GLFrameBuffer(
        uint32_t width,
        uint32_t height) :
        gl_internal_format(GL_RGBA32F),
        gl_format(GL_RGBA),
        gl_type(GL_FLOAT),
        TextureTyped<TextureDataFloat>(
            width,
            height,
            nullptr)
    {
        Create();
    }

    template <typename T>
    GLFrameBuffer<T>::~GLFrameBuffer()
    {
        glDeleteTextures(
            1, &gl_texture_handle);
    }

    template <typename T>
    void GLFrameBuffer<T>::Create()
    {
        glGenFramebuffers(
            1,
            &gl_frame_buffer);

        glBindFramebuffer(
            GL_FRAMEBUFFER,
            gl_frame_buffer);

        glGenTextures(
            1,
            &gl_texture_handle);

        glBindTexture(
            GL_TEXTURE_2D,
            gl_texture_handle);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            gl_internal_format,
            Texture::width,
            Texture::height,
            0,
            gl_format,
            gl_type,
            0);

        glGenRenderbuffers(
            1,
            &gl_depth_renderbuffer);

        glBindRenderbuffer(
            GL_RENDERBUFFER,
            gl_depth_renderbuffer);

        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT24,
            Texture::width,
            Texture::height);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "OpenGL error: " << err << std::endl;
        }

        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            gl_depth_renderbuffer);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            gl_texture_handle,
            0);

        GLenum draw_buffers[1] =
        {
            GL_COLOR_ATTACHMENT0
        };

        glDrawBuffers(
            1,
            draw_buffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            assert(false);
        }

        glBindFramebuffer(
            GL_FRAMEBUFFER,
            0);
    }

    template <typename T>
    void GLFrameBuffer<T>::Bind()
    {
        glBindFramebuffer(
            GL_FRAMEBUFFER,
            gl_frame_buffer);

        glViewport(
            0,
            0,
            Texture::width,
            Texture::height);

        glClearColor(
            0, 0, 0, 0);

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT);
    }

    template <typename T>
    void GLFrameBuffer<T>::Unbind()
    {
        glBindFramebuffer(
            GL_FRAMEBUFFER,
            0);

        //glViewport(
        //    0,
        //    0,
        //    window_width,
        //    window_height);
    }

    template <typename T>
    void GLFrameBuffer<T>::GenerateMipMaps()
    {
        glActiveTexture(
            GL_TEXTURE0);

        glBindTexture(
            GL_TEXTURE_2D,
            gl_texture_handle);

        glGenerateMipmap(
            GL_TEXTURE_2D);
    }
}
}
}

#include "GLTexture.hpp"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    template <>
    GLTexture<TextureDataByte>::GLTexture(
        uint32_t width,
        uint32_t height,
        TextureDataRGBA8Ptr data) :
        gl_internal_format(GL_RGBA8),
        gl_format(GL_RGBA),
        gl_type(GL_UNSIGNED_BYTE),
        TextureTyped<TextureDataByte>(
            width,
            height,
            data)
    {
        Create();
    }

    template <>
    GLTexture<TextureDataFloat>::GLTexture(
        uint32_t width,
        uint32_t height,
        TextureDataRGBA32FPtr data) :
        gl_internal_format(GL_RGBA32F),
        gl_format(GL_RGBA),
        gl_type(GL_FLOAT),
        TextureTyped<TextureDataFloat>(
            width,
            height,
            data)
    {
        Create();
    }

    template <typename T>
    GLTexture<T>::~GLTexture()
    {
        glDeleteTextures(
            1, &gl_texture_handle);
    }

    template <typename T>
    void GLTexture<T>::Create()
    {
        glActiveTexture(
            GL_TEXTURE0);

        glGenTextures(
            1, &gl_texture_handle);

        glBindTexture(
            GL_TEXTURE_2D,
            gl_texture_handle);

        T* data_ptr = &(*TextureTyped<T>::data)[0];
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            gl_internal_format,
            Texture::width,
            Texture::height,
            0,
            gl_format,
            gl_type,
            (GLvoid*)data_ptr);

        glGenerateMipmap(
            GL_TEXTURE_2D);

        glBindTexture(
            GL_TEXTURE_2D,
            NULL);

        Texture::invalidated = false;
    }

    template <typename T>
    void GLTexture<T>::Update()
    {
        if (Texture::invalidated)
        {
            glActiveTexture(
                GL_TEXTURE0);

            glBindTexture(
                GL_TEXTURE_2D,
                gl_texture_handle);

            T* data_ptr = &(*TextureTyped<T>::data)[0];
            glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                0,
                0,
                Texture::width,
                Texture::height,
                gl_format,
                gl_type,
                (GLvoid*)data_ptr);

            glGenerateMipmap(
                GL_TEXTURE_2D);

            glBindTexture(
                GL_TEXTURE_2D,
                NULL);

            Texture::invalidated = false;
        }
    }

    template class GLTexture<TextureDataByte>;
    template class GLTexture<TextureDataFloat>;
}
}
}

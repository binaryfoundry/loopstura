#include "GLTexture.hpp"

namespace Application
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
        Texture<TextureDataByte>(
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
        Texture<TextureDataFloat>(
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

        Update();
    }

    template <typename T>
    void GLTexture<T>::Update()
    {
        if (Texture<T>::invalidated)
        {
            glActiveTexture(
                GL_TEXTURE0);

            glBindTexture(
                GL_TEXTURE_2D,
                gl_texture_handle);

            T* data_ptr = &(*Texture<T>::data)[0];
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                gl_internal_format,
                Texture<T>::width,
                Texture<T>::height,
                0,
                gl_format,
                gl_type,
                (GLvoid*)data_ptr);

            glGenerateMipmap(
                GL_TEXTURE_2D);

            Texture<T>::invalidated = false;

            glBindTexture(
                GL_TEXTURE_2D,
                NULL);
        }
    }

    template class GLTexture<TextureDataByte>;
    template class GLTexture<TextureDataFloat>;
}
}

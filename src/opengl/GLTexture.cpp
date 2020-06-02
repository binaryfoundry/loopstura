#include "GLTexture.hpp"

namespace Application
{
namespace OpenGL
{
    template <>
    GLTexture<TextureDataByteRGBA>::GLTexture(
        uint32_t width,
        uint32_t height) :
        gl_internal_format(GL_RGBA8),
        gl_format(GL_RGBA),
        gl_type(GL_UNSIGNED_BYTE),
        Texture<TextureDataByteRGBA>(
            width,
            height)
    {
        Create();
    }

    template <>
    GLTexture<TextureDataFloatRGBA>::GLTexture(
        uint32_t width,
        uint32_t height) :
        gl_internal_format(GL_RGBA32F),
        gl_format(GL_RGBA),
        gl_type(GL_FLOAT),
        Texture<TextureDataFloatRGBA>(
            width,
            height)
    {
        Create();
    }

    template <typename T>
    GLTexture<T>::~GLTexture()
    {
        glDeleteTextures(
            1, &texture);
    }

    template <typename T>
    void GLTexture<T>::Create()
    {
        glActiveTexture(
            GL_TEXTURE0);

        glGenTextures(
            1, &texture);

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
                texture);

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

    template <typename T>
    void GLTexture<T>::Bind()
    {
        Update();

        glActiveTexture(
            GL_TEXTURE0);

        glBindTexture(
            GL_TEXTURE_2D,
            texture);
    }

    template <typename T>
    void GLTexture<T>::Unbind()
    {
        glBindTexture(
            GL_TEXTURE_2D,
            NULL);
    }

    template class GLTexture<TextureDataByteRGBA>;
    template class GLTexture<TextureDataFloatRGBA>;
}
}

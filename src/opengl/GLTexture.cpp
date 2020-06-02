#include "GLTexture.hpp"

namespace Application
{
namespace OpenGL
{
    template <>
    GLTexture<TexureDataByteRGBA>::GLTexture(
        uint32_t width,
        uint32_t height) :
        gl_internal_format(GL_RGBA8),
        gl_format(GL_RGBA),
        gl_type(GL_UNSIGNED_BYTE),
        Texture<TexureDataByteRGBA>(
            width,
            height)
    {
        Create();
    }

    template <>
    GLTexture<TexureDataFloatRGBA>::GLTexture(
        uint32_t width,
        uint32_t height) :
        gl_internal_format(GL_RGBA32F),
        gl_format(GL_RGBA),
        gl_type(GL_FLOAT),
        Texture<TexureDataFloatRGBA>(
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
        Bind();

        if (true) // TODO if texture data is updated
        {
            T* data_ptr = &(*data)[0];
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                gl_internal_format,
                width,
                height,
                0,
                gl_format,
                gl_type,
                (GLvoid*)data_ptr);

            glGenerateMipmap(
                GL_TEXTURE_2D);
        }

        Unbind();
    }

    template <typename T>
    void GLTexture<T>::Bind()
    {
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

    template class GLTexture<TexureDataByteRGBA>;
    template class GLTexture<TexureDataFloatRGBA>;
}
}

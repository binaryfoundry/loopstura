#include "GLTexture.hpp"

namespace Application
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
    }

    template <typename T>
    GLTexture<T>::~GLTexture()
    {
    }

    template class GLTexture<TexureDataByteRGBA>;
    template class GLTexture<TexureDataFloatRGBA>;
}


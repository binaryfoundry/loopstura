#pragma once

#include "GL.hpp"

#include "../Texture.hpp"

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
    template <typename T>
    class GLTexture : public Texture<T>
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        GLTexture(
            uint32_t width,
            uint32_t height);

        const GLint gl_internal_format = 0;
        const GLenum gl_format = 0;
        const GLenum gl_type = 0;

    public:
        virtual ~GLTexture();
    };
}
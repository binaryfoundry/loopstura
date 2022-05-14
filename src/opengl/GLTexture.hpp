#pragma once

#include "GL.hpp"

#include "../rendering/Texture.hpp"

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
namespace OpenGL
{
    class GLTextureHandle
    {
    public:
        GLuint gl_texture_handle = 0; // TODO make private
    };

    template <typename T>
    class GLTexture : public Texture<T>, public GLTextureHandle
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        const GLint gl_internal_format = 0;
        const GLenum gl_format = 0;
        const GLenum gl_type = 0;

        void Create();

    public:
        GLTexture(
            uint32_t width,
            uint32_t height,
            std::shared_ptr<std::vector<T>> data);
        virtual ~GLTexture();

        void Update() override;
    };
}
}

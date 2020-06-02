#pragma once

#include "../GL.hpp"
#include "../GLShader.hpp"
#include "../GLTexture.hpp"

namespace Application
{
namespace OpenGL
{
    class GLShaderBasic : public GLShader
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture;

        GLuint& shader_program;
        GLuint texture_uniform_location = 0;
        GLuint sampler_state = 0;

    public:
        GLShaderBasic(
            GLuint& shader_program,
            std::shared_ptr<Texture<TextureDataByteRGBA>> texture);
        virtual ~GLShaderBasic();

        void Bind();
        void Unbind();
    };
}
}

#pragma once

#include "../GL.hpp"
#include "../GLMaterial.hpp"
#include "../GLTexture.hpp"

namespace Application
{
namespace OpenGL
{
    class GLMaterialBasic : public GLMaterial
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture;

        GLuint& shader_program;
        GLuint texture_uniform_location = 0;
        GLuint sampler_state = 0;

    public:
        GLMaterialBasic(
            GLuint& shader_program,
            std::shared_ptr<Texture<TextureDataByteRGBA>> texture);
        virtual ~GLMaterialBasic();

        void Bind();
        void Unbind();
    };
}
}

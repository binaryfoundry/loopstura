#pragma once

#include "../GL.hpp"
#include "../GLInstance.hpp"
#include "../GLTexture.hpp"

namespace Application
{
namespace OpenGL
{
    class GLInstanceBasic : public GLInstance
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture;

        GLuint texture_uniform_location = 0;
        GLuint sampler_state = 0;

    public:
        GLInstanceBasic(
            GLuint& shader_program,
            std::vector<GLfloat>& vertices,
            std::vector<GLuint>& indices,
            std::shared_ptr<Texture<TextureDataByteRGBA>> texture);
        virtual ~GLInstanceBasic();

        void Draw();
    };
}
}

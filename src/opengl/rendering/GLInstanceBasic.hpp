#pragma once

#include "../GL.hpp"
#include "../GLInstance.hpp"
#include "../GLTexture.hpp"

#include "../../rendering/InstanceBasic.hpp"

namespace Application
{
namespace OpenGL
{
    class GLInstanceBasic : public Rendering::InstanceBasic, public GLInstance
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:

        static GLuint shader_program;

        GLuint texture_uniform_location = 0;
        GLuint sampler_state = 0;

    public:
        GLInstanceBasic(
            std::vector<float>& vertices,
            std::vector<uint32_t>& indices,
            std::shared_ptr<Texture<TextureDataByteRGBA>> texture);
        virtual ~GLInstanceBasic();

        static void Initialise();
        static void Destroy();

        void Draw();
    };
}
}

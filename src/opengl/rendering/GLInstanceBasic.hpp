#pragma once

#include "../GL.hpp"
#include "../GLTexture.hpp"
#include "../GLShader.hpp"

#include "../../rendering/InstanceBasic.hpp"

namespace Application
{
namespace OpenGL
{
    class GLInstanceBasic : public Rendering::InstanceBasic
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        static GLuint shader_program;

        GLuint texture_uniform_location = 0;
        GLuint fade_uniform_location = 0;
        GLuint sampler_state = 0;

        GLuint gl_vertex_buffer = 0;
        GLuint gl_index_buffer = 0;

    public:
        GLInstanceBasic(
            ContextPtr context,
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

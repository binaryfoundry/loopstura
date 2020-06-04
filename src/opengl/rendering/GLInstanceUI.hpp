#pragma once

#include "../GL.hpp"
#include "../GLTexture.hpp"
#include "../GLShader.hpp"

#include "../../rendering/InstanceUI.hpp"

namespace Application
{
namespace OpenGL
{
    class GLInstanceUI : public Rendering::InstanceUI
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        static GLuint gl_shader_program;

        GLuint gl_texture_uniform_location = 0;
        GLuint gl_fade_uniform_location = 0;
        GLuint gl_sampler_state = 0;

        GLuint gl_vertex_buffer = 0;
        GLuint gl_index_buffer = 0;

    public:
        GLInstanceUI(
            ContextPtr context,
            std::vector<float>& vertices,
            std::vector<uint32_t>& indices,
            std::shared_ptr<Texture<TextureDataByteRGBA>> texture);
        virtual ~GLInstanceUI();

        static void Initialise();
        static void Destroy();

        void Draw();
    };
}
}

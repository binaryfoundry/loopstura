#pragma once

#include "GL.hpp"
#include "GLTexture.hpp"

#include "../rendering/Renderer.hpp"
#include "../rendering/UserInterface.hpp"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    class GLUserInterface : public UserInterface
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        static GLuint gl_shader_program;

        GLuint gl_projection_uniform_location = 0;
        GLuint gl_view_uniform_location = 0;
        GLuint gl_viewport_uniform_location = 0;
        GLuint gl_texture_uniform_location = 0;
        GLuint gl_brightness_uniform_location = 0;
        GLuint gl_gradient_uniform_location = 0;
        GLuint gl_gradient_0_uniform_location = 0;
        GLuint gl_gradient_1_uniform_location = 0;

        GLuint gl_sampler_state = 0;

    public:
        GLUserInterface(
            ContextPtr context,
            VertexStreamPtr vertices,
            IndexStreamPtr indices,
            TextureRGBA8Ptr texture);
        virtual ~GLUserInterface();

        static void Initialise();
        static void Destroy();

        void Draw(RenderState state);
    };
}
}
}

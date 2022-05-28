#pragma once

#include "GL.hpp"
#include "GLTexture.hpp"

#include "../rendering/Renderer.hpp"
#include "../rendering/Imgui.hpp"
#include "../rendering/Instance.hpp"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    class GLImgui : public Imgui
    {
        class GLRenderer;
        friend class GLRenderer;

    protected:
        static GLuint gl_shader_program;

        GLuint gl_sampler_state = 0;

        GLuint g_AttribLocationTex;
        GLuint g_AttribLocationProjMtx;
        GLuint g_AttribLocationVtxPos;
        GLuint g_AttribLocationVtxUV;
        GLuint g_AttribLocationVtxColor;

        unsigned int g_VboHandle = 0, g_ElementsHandle = 0;

        GLuint g_FontTexture = 0;

    public:
        GLImgui(
            ContextPtr context);
        virtual ~GLImgui();

        static void Initialise();
        static void Destroy();

        void Draw(RenderState state);
    };
}
}
}

#include "GLImgui.hpp"

#include <string>
#include <iostream>

#include "GLStream.hpp"

#include "imgui/imgui.h"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    static std::string quad_vertex_shader_string =
        R"(#version 300 es
        #ifdef GL_ES
        precision mediump float;
        #endif
        layout (location = 0) in vec2 Position;
        layout (location = 1) in vec2 UV;
        layout (location = 2) in vec4 Color;
        uniform mat4 ProjMtx;
        out vec2 Frag_UV;
        out vec4 Frag_Color;
        void main()
        {
            Frag_UV = UV;
            Frag_Color = Color;
            gl_Position = ProjMtx * vec4(Position.xy,0,1);
        })";

    static std::string quad_fragment_shader_string =
        R"(#version 300 es
        #ifdef GL_ES
        precision mediump float;
        #endif
        uniform sampler2D Texture;
        in vec2 Frag_UV;
        in vec4 Frag_Color;
        layout (location = 0) out vec4 Out_Color;
        void main()
        {
            Out_Color = Frag_Color * texture(Texture, Frag_UV.st);
        })";

    GLuint GLImgui::gl_shader_program = 0;

    void GLImgui::Initialise()
    {
        gl_shader_program = LinkShader(
            quad_vertex_shader_string,
            quad_fragment_shader_string);
    }

    void GLImgui::Destroy()
    {
        glDeleteProgram(
            gl_shader_program);
    }

    GLImgui::GLImgui(
        ContextPtr context)
    {
        g_AttribLocationTex = glGetUniformLocation(gl_shader_program, "Texture");
        g_AttribLocationProjMtx = glGetUniformLocation(gl_shader_program, "ProjMtx");
        g_AttribLocationVtxPos = (GLuint)glGetAttribLocation(gl_shader_program, "Position");
        g_AttribLocationVtxUV = (GLuint)glGetAttribLocation(gl_shader_program, "UV");
        g_AttribLocationVtxColor = (GLuint)glGetAttribLocation(gl_shader_program, "Color");

        glGenBuffers(1, &g_VboHandle);
        glGenBuffers(1, &g_ElementsHandle);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(1280, 720);
        io.DeltaTime = 1.0f / 60.0f;

        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(
            &pixels,
            &width,
            &height);

        glGenTextures(
            1,
            &g_FontTexture);

        glBindTexture(
            GL_TEXTURE_2D,
            g_FontTexture);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            pixels);

        io.Fonts->TexID = (ImTextureID)(intptr_t)g_FontTexture;

        glGenSamplers(
            1, &gl_sampler_state);

        glActiveTexture(
            GL_TEXTURE0);

        glSamplerParameteri(
            gl_sampler_state,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);

        glSamplerParameteri(
            gl_sampler_state,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);

        glSamplerParameteri(
            gl_sampler_state,
            GL_TEXTURE_MAG_FILTER,
            GL_NEAREST);

        glSamplerParameteri(
            gl_sampler_state,
            GL_TEXTURE_MIN_FILTER,
            GL_NEAREST);
    }

    GLImgui::~GLImgui()
    {
        ImGui::DestroyContext();

        glDeleteTextures(
            1,
            &g_FontTexture);

        glDeleteSamplers(
            1,
            &gl_sampler_state);
    }

    void GLImgui::Draw(RenderState state)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);

        float fb_width = state.viewport.z;
        float fb_height = state.viewport.w;

        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();

        glActiveTexture(
            GL_TEXTURE0);

        glUseProgram(
            gl_shader_program);

        glUniform1i(
            g_AttribLocationTex,
            0);

        glUniformMatrix4fv(
            g_AttribLocationProjMtx,
            1,
            GL_FALSE,
            &state.projection[0][0]);

        glBindSampler(
            0,
            gl_sampler_state);

        glBindBuffer(
            GL_ARRAY_BUFFER,
            g_VboHandle);

        glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER,
            g_ElementsHandle);

        glEnableVertexAttribArray(
            g_AttribLocationVtxPos);

        glEnableVertexAttribArray(
            g_AttribLocationVtxUV);

        glEnableVertexAttribArray(
            g_AttribLocationVtxColor);

        glVertexAttribPointer(
            g_AttribLocationVtxPos,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(ImDrawVert),
            (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));

        glVertexAttribPointer(
            g_AttribLocationVtxUV,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(ImDrawVert),
            (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));

        glVertexAttribPointer(
            g_AttribLocationVtxColor,
            4,
            GL_UNSIGNED_BYTE,
            GL_TRUE,
            sizeof(ImDrawVert),
            (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

        ImVec2 clip_off = draw_data->DisplayPos;
        ImVec2 clip_scale = draw_data->FramebufferScale;

        for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];

            glBufferData(
                GL_ARRAY_BUFFER,
                (GLsizeiptr)cmd_list->VtxBuffer.Size * (int)sizeof(ImDrawVert),
                (const GLvoid*)cmd_list->VtxBuffer.Data,
                GL_STREAM_DRAW);

            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                (GLsizeiptr)cmd_list->IdxBuffer.Size * (int)sizeof(ImDrawIdx),
                (const GLvoid*)cmd_list->IdxBuffer.Data,
                GL_STREAM_DRAW);

            for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
            {
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width &&
                    clip_rect.y < fb_height &&
                    clip_rect.z >= 0.0f &&
                    clip_rect.w >= 0.0f)
                {
                    glScissor(
                        (int)clip_rect.x,
                        (int)(fb_height - clip_rect.w),
                        (int)(clip_rect.z - clip_rect.x),
                        (int)(clip_rect.w - clip_rect.y));

                    glBindTexture(
                        GL_TEXTURE_2D,
                        (GLuint)(intptr_t)pcmd->TextureId);

                    glDrawElements(
                        GL_TRIANGLES,
                        (GLsizei)pcmd->ElemCount,
                        sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                        (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)));
                }
            }
        }

        glDisable(GL_BLEND);
        glDisable(GL_SCISSOR_TEST);
    }
}
}
}

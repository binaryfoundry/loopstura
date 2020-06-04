#include "GLInstanceUI.hpp"

#include <string>
#include <iostream>

#include "../GLShader.hpp"

namespace Application
{
namespace OpenGL
{
    static std::string vertex_shader_string =
        R"(#version 300 es
        #ifdef GL_ES
        precision mediump float;
        #endif
        layout(location = 0) in vec3 v_position;
        layout(location = 1) in vec2 texcoord;
        out vec2 v_texcoord;
        void main()
        {
            v_texcoord = texcoord;
            gl_Position = vec4(v_position, 1.0);
        })";

    static std::string fragment_shader_string =
        R"(#version 300 es
        #ifdef GL_ES
        precision mediump float;
        #endif
        in vec2 v_texcoord;
        layout(location = 0) out vec4 out_color;
        uniform sampler2D tex;
        uniform float fade;
        void main()
        {
            vec4 c = texture(tex, v_texcoord) * fade;
            out_color = vec4(c.xyz, 1.0);
        })";

    GLuint GLInstanceUI::gl_shader_program = 0;

    void GLInstanceUI::Initialise()
    {
        gl_shader_program = LinkShader(
            vertex_shader_string,
            fragment_shader_string);
    }

    void GLInstanceUI::Destroy()
    {
        glDeleteProgram(
            gl_shader_program);
    }

    GLInstanceUI::GLInstanceUI(
        ContextPtr context,
        std::vector<float>& vertices,
        std::vector<uint32_t>& indices,
        TextureRGBA8Ptr texture) :
        InstanceUI(
            context,
            vertices,
            indices,
            texture)
    {
        gl_vertex_buffer = GenBuffer(
            vertices,
            GL_ARRAY_BUFFER,
            GL_STATIC_DRAW);

        gl_index_buffer = GenBuffer(
            indices,
            GL_ELEMENT_ARRAY_BUFFER,
            GL_STATIC_DRAW);

        gl_texture_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "tex");

        gl_fade_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "fade");

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
            GL_LINEAR_MIPMAP_LINEAR);
    }

    GLInstanceUI::~GLInstanceUI()
    {
        glDeleteSamplers(
            1,
            &gl_sampler_state);
    }

    void GLInstanceUI::Draw()
    {
        glActiveTexture(
            GL_TEXTURE0);

        glUseProgram(
            gl_shader_program);

        glBindSampler(
            0,
            gl_sampler_state);

        texture->Update();

        auto gl_texture_handle = std::dynamic_pointer_cast<GLTextureHandle>(
            texture)->gl_texture_handle;

        glActiveTexture(
            GL_TEXTURE0);

        glBindTexture(
            GL_TEXTURE_2D,
            gl_texture_handle);

        glUniform1i(
            gl_texture_uniform_location,
            0);

        glUniform1f(
            gl_fade_uniform_location,
            fade->Value());

        glBindBuffer(
            GL_ARRAY_BUFFER,
            gl_vertex_buffer);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(GLfloat),
            (GLvoid*)0);

        glEnableVertexAttribArray(1);

        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(GLfloat),
            (GLvoid*)(3 * sizeof(GLfloat)));

        glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER,
            gl_index_buffer);

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(indices.size()),
            GL_UNSIGNED_INT,
            static_cast<char const*>(0));

        glBindTexture(
            GL_TEXTURE_2D,
            NULL);
    }
}
}

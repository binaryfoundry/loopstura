#include "GLShaderBasic.hpp"

#include <string>
#include <iostream>

namespace Application
{
namespace OpenGL
{
    std::string vertex_shader_string =
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

    std::string fragment_shader_string =
        R"(#version 300 es
        #ifdef GL_ES
        precision mediump float;
        #endif
        in vec2 v_texcoord;
        layout(location = 0) out vec4 out_color;
        uniform sampler2D tex;
        void main()
        {
            vec4 c = texture(tex, v_texcoord);
            out_color = vec4(c.xyz, 1.0);
        })";

    GLShaderBasic::GLShaderBasic(
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture) :
        texture(texture)
    {
        // TODO single instance of object
        shader = LinkShader(
            vertex_shader_string,
            fragment_shader_string);

        texture_uniform_location = glGetUniformLocation(
            shader,
            "tex");

        glGenSamplers(
            1, &sampler_state);

        glActiveTexture(
            GL_TEXTURE0);

        glSamplerParameteri(
            sampler_state,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);

        glSamplerParameteri(
            sampler_state,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);

        glSamplerParameteri(
            sampler_state,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);

        glSamplerParameteri(
            sampler_state,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    GLShaderBasic::~GLShaderBasic()
    {
        glDeleteProgram(
            shader);

        glDeleteSamplers(
            1, &sampler_state);
    }

    void GLShaderBasic::Bind()
    {
        glActiveTexture(
            GL_TEXTURE0);

        auto gl_texture = std::dynamic_pointer_cast<GLBindable>(texture);
        gl_texture->Bind();

        glUseProgram(
            shader);

        glBindSampler(
            0,
            sampler_state);

        glUniform1i(
            0,
            texture_uniform_location);
    }

    void GLShaderBasic::Unbind()
    {
        auto gl_texture = std::dynamic_pointer_cast<GLBindable>(texture);

        gl_texture->Unbind();
    }
}
}

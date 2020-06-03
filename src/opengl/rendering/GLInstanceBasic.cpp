#include "GLInstanceBasic.hpp"

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
        void main()
        {
            vec4 c = texture(tex, v_texcoord);
            out_color = vec4(c.xyz, 1.0);
        })";

    GLuint GLInstanceBasic::shader_program = 0;

    void GLInstanceBasic::Initialise()
    {
        shader_program = LinkShader(
            vertex_shader_string,
            fragment_shader_string);
    }

    void GLInstanceBasic::Destroy()
    {
        glDeleteProgram(
            shader_program);
    }

    GLInstanceBasic::GLInstanceBasic(
        std::vector<float>& vertices,
        std::vector<uint32_t>& indices,
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture) :
        InstanceBasic(
            texture),
        GLInstance(
            vertices,
            indices)
    {
        texture_uniform_location = glGetUniformLocation(
            shader_program,
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
            GL_NEAREST);

        glSamplerParameteri(
            sampler_state,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    GLInstanceBasic::~GLInstanceBasic()
    {

        glDeleteSamplers(
            1, &sampler_state);
    }

    void GLInstanceBasic::Draw()
    {
        glActiveTexture(
            GL_TEXTURE0);

        auto gl_texture = std::dynamic_pointer_cast<GLBindable>(texture);
        gl_texture->Bind();

        glUseProgram(
            shader_program);

        glBindSampler(
            0,
            sampler_state);

        glUniform1i(
            0,
            texture_uniform_location);

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

        gl_texture->Unbind();
    }
}
}

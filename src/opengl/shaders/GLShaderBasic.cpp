#include "GLShaderBasic.hpp"

#include <string>
#include <iostream>

namespace Application
{
namespace OpenGL
{
    GLShaderBasic::GLShaderBasic(
        GLuint& shader_program,
        std::shared_ptr<Texture<TextureDataByteRGBA>> texture) :
        shader_program(shader_program),
        texture(texture)
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
            GL_LINEAR);

        glSamplerParameteri(
            sampler_state,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    GLShaderBasic::~GLShaderBasic()
    {
        glDeleteProgram(
            shader_program);

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
            shader_program);

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

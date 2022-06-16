#include "GLEnvironmentInstance.hpp"

#include <string>
#include <iostream>

#include "../GLStream.hpp"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    static std::string vertex_shader_string =
        R"(#version 300 es
        #ifdef GL_ES
        precision mediump float;
        #endif
        uniform mat4 projection;
        uniform mat4 view;
        uniform vec4 viewport;
        layout(location = 0) in vec3 v_position;
        layout(location = 1) in vec2 texcoord;
        out vec2 v_texcoord;
        void main() {
            v_texcoord = texcoord;
            gl_Position = vec4(v_position, 1.0);
        })";

    static std::string fragment_shader_string =
        R"(#version 300 es
        #ifdef GL_ES
        precision mediump float;
        #endif
        vec3 linear(vec3 v) { return pow(v, vec3(2.2)); }
        vec3 gamma(vec3 v) { return pow(v, 1.0 / vec3(2.2)); }
        in vec2 v_texcoord;
        layout(location = 0) out vec4 out_color;
        void main() {
            vec3 c = linear(abs(v_texcoord.xyy));
            out_color = vec4(gamma(c), 1.0);
        })";

    GLEnvironmentInstance::GLEnvironmentInstance(
        std::shared_ptr<GLStream<float>> quad_vertices,
        std::shared_ptr<GLStream<uint32_t>> quad_indices) :
        quad_vertices(quad_vertices),
        quad_indices(quad_indices)
    {
        gl_shader_program = LinkShader(
            vertex_shader_string,
            fragment_shader_string);

        gl_projection_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "projection");

        gl_view_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "view");

        gl_viewport_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "viewport");

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

    GLEnvironmentInstance::~GLEnvironmentInstance()
    {
        glDeleteSamplers(
            1,
            &gl_sampler_state);
    }

    void GLEnvironmentInstance::Bind(RenderState state)
    {
        glUseProgram(
            gl_shader_program);

        quad_vertices->Update();
        quad_indices->Update();

        const GLuint gl_vertex_buffer = quad_vertices->gl_buffer_handle;

        const GLuint gl_index_buffer = quad_indices->gl_buffer_handle;

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
    }

    void GLEnvironmentInstance::Unbind()
    {
        glBindTexture(
            GL_TEXTURE_2D,
            NULL);
    }

    void GLEnvironmentInstance::Draw(RenderState state)
    {
        mat4 projection = glm::ortho<float>(
            -1.0f,
            1.0f,
            1.0f,
            -1.0f,
            -1.0f,
            1.0f);

        glUniformMatrix4fv(
            gl_projection_uniform_location,
            1,
            false,
            &projection[0][0]);

        glUniformMatrix4fv(
            gl_view_uniform_location,
            1,
            false,
            &state.view[0][0]);

        glUniform4fv(
            gl_viewport_uniform_location,
            1,
            &state.viewport[0]);

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(quad_indices->data->size()),
            GL_UNSIGNED_INT,
            static_cast<char const*>(0));
    }
}
}
}

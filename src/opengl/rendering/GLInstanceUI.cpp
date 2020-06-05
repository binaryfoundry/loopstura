#include "GLInstanceUI.hpp"

#include <string>
#include <iostream>

#include "../GLStream.hpp"

namespace Application
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
        void main()
        {
            v_texcoord = texcoord;
            gl_Position = projection * view * vec4(v_position, 1.0);
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
        uniform sampler2D tex;
        uniform float brightness;
        uniform float gradient;
        uniform vec3 gradient_0;
        uniform vec3 gradient_1;
        void main()
        {
            vec3 c = linear(texture(tex, v_texcoord).xyz);
            c = mix(c, mix(linear(gradient_0), linear(gradient_1), v_texcoord.y), gradient);
            c = mix(c, vec3(1.0), clamp(brightness, 0.0, 1.0));
            c = mix(c, vec3(0.0), clamp(-brightness, 0.0, 1.0));
            out_color = vec4(gamma(c), 1.0);
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
        VertexStreamPtr vertices,
        IndexStreamPtr indices,
        TextureRGBA8Ptr texture) :
        InstanceUI(
            context,
            vertices,
            indices,
            texture)
    {
        gl_projection_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "projection");

        gl_view_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "view");

        gl_viewport_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "viewport");

        gl_texture_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "tex");

        gl_brightness_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "brightness");

        gl_gradient_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "gradient");

        gl_gradient_0_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "gradient_0");

        gl_gradient_1_uniform_location = glGetUniformLocation(
            gl_shader_program,
            "gradient_1");

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

    void GLInstanceUI::Draw(RenderState state)
    {
        glUseProgram(
            gl_shader_program);

        glUniformMatrix4fv(
            gl_projection_uniform_location,
            1,
            false,
            &state.projection[0][0]);

        glUniformMatrix4fv(
            gl_view_uniform_location,
            1,
            false,
            &state.view[0][0]);

        glUniform4fv(
            gl_viewport_uniform_location,
            1,
            &state.viewport[0]);

        glUniform1f(
            gl_brightness_uniform_location,
            brightness->Value());

        glUniform1f(
            gl_gradient_uniform_location,
            gradient->Value());

        glUniform3fv(
            gl_gradient_0_uniform_location,
            1,
            &gradient_0->Value()[0]);

        glUniform3fv(
            gl_gradient_1_uniform_location,
            1,
            &gradient_1->Value()[0]);

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

        glBindSampler(
            0,
            gl_sampler_state);

        vertices->Update();
        indices->Update();

        GLuint gl_vertex_buffer = std::dynamic_pointer_cast<GLBufferHandle>(
            vertices)->gl_buffer_handle;

        GLuint gl_index_buffer = std::dynamic_pointer_cast<GLBufferHandle>(
            indices)->gl_buffer_handle;

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
            static_cast<GLsizei>(indices->data->size()),
            GL_UNSIGNED_INT,
            static_cast<char const*>(0));

        glBindTexture(
            GL_TEXTURE_2D,
            NULL);
    }
}
}

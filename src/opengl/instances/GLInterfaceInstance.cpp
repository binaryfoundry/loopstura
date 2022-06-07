#include <string>

#include "GLInterfaceInstance.hpp"

#include "../GLTexture.hpp"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    static const std::string quad_vertex_shader_string =
        R"(#version 300 es
        #extension GL_OES_standard_derivatives : enable
        #ifdef GL_ES
        precision mediump float;
        #endif
        uniform mat4 model;
        uniform mat4 projection;
        uniform mat4 view;
        uniform vec4 viewport;
        layout(location = 0) in vec3 v_position;
        layout(location = 1) in vec2 texcoord;
        out vec2 v_texcoord;
        void main() {
            v_texcoord = texcoord;
            gl_Position = projection * view * model * vec4(v_position, 1.0);
        })";

    static const std::string quad_fragment_shader_string =
        R"(#version 300 es
        #ifdef GL_ES
        precision mediump float;
        #endif
        vec3 linear(vec3 v) { return pow(v, vec3(2.2)); }
        vec4 linear(vec4 v) { return vec4(pow(v.xyz, vec3(2.2)), v.w); }
        vec3 gamma(vec3 v) { return pow(v, 1.0 / vec3(2.2)); }
        in vec2 v_texcoord;
        layout(location = 0) out vec4 out_color;
        uniform sampler2D tex;
        uniform float tex_blend;
        uniform vec2 tex_scale;
        uniform int sdf_func;
        uniform float nonlinearity;
        uniform float brightness;
        uniform vec3 gradient_0;
        uniform vec3 gradient_1;
        uniform float alpha_margin;
        float buff = 1.0;
        float sdCircle(in vec2 p, in float r) {
            return length(p) - r;
        }
        vec3 sdgCircle(in vec2 p, in float r) {
            float d = length(p);
            return vec3( d-r, p/d );
        }
        float sdBox(in vec2 p, in vec2 b)  {
            vec2 d = abs(p) - b;
            return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
        }
        vec3 sdgBox(in vec2 p, in vec2 b) {
            vec2 w = abs(p) - b;
            vec2 s = vec2(p.x < 0.0 ? -1 : 1, p.y < 0.0 ? -1 : 1);
            float g = max(w.x, w.y);
            vec2  q = max(w, 0.0);
            float l = length(q);
            return vec3((g > 0.0) ? l : g,
                        s * ((g > 0.0) ? q / l : ((w.x > w.y) ? vec2(1, 0) : vec2(0, 1))));
        }
        float sdRoundedBox(in vec2 p, in vec2 b, in float r) {
            vec2 q = abs(p) -b + r;
            return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r;
        }
        vec3 sdgRound(in vec3 n, in float r) {
            return vec3(n.x - r, n.yz);
        }
        void main() {
            float d; // SDF value
            vec3 n = vec3(1.0, 0.0, 0.0); //  SDF Normal
            vec2 tc = v_texcoord.xy * tex_scale; // texture coordinates
            vec3 s = texture(tex, tc).xyz; // s = texture sample
            if (sdf_func == 0) {
                d = sdBox(tc - vec2(0.5), vec2(0.5));
            }
            else if (sdf_func == 1) {
                vec2 p = tc - vec2(0.5);
                float r = 0.5;
                d = sdCircle(p, r);
                n = sdgCircle(p, r);
            }
            else if (sdf_func == 2) {
                float rad = 0.2;
                vec2 p = tc - vec2(0.5);
                vec2 b = vec2(0.5);
                d = sdRoundedBox(p, b, rad);
                n = sdgRound(sdgBox(p, b), rad);
            }
            else if (sdf_func == 3) { // Cylinder
                vec2 p = vec2(0.5, tc.y) - vec2(0.5);
                vec2 b = vec2(0.5);
                d = sdBox(p, b);
                n = sdgBox(p, b);
            }
            else if (sdf_func == 4) { // Waveform
               float wmax = (s.x + 1.0) * 0.5;
               float wmin = (((s.y) + 1.0) * 0.5) - 1.0;
               float wv = tc.y < 0.5 ? wmax : 1.0 - wmin;
               d = sdBox(vec2(0.5, tc.y - (1.0 - wv)) - vec2(0.5), vec2(0.5));
            }
            n = normalize(n.yzx);
            float e = length(vec2(dFdx(d), dFdy(d)));
            float alpha_width = alpha_margin * e;
            float alpha = smoothstep(buff - alpha_width, buff, 1.0 - d);
            alpha = d > 0.0 ? alpha : 1.0; // clamp to outside SDF shape
            float dr = 1.0 - abs(d);
            float nmx = nonlinearity > 0.0 ? 1.0 - pow(dr, nonlinearity) : pow(dr, -nonlinearity);
            vec3 c = mix(linear(gradient_0), linear(gradient_1), nmx);
            c = mix(c, linear(s).xyz, tex_blend);
            c = mix(c, vec3(1.0), clamp(brightness, 0.0, 1.0));
            c = mix(c, vec3(0.0), clamp(-brightness, 0.0, 1.0));
            out_color = vec4(gamma(c), alpha);
        })";

    GLInterfaceInstance::GLInterfaceInstance(
        std::shared_ptr<GLStream<float>> quad_vertices,
        std::shared_ptr<GLStream<uint32_t>> quad_indices) :
        quad_vertices(quad_vertices),
        quad_indices(quad_indices)
    {
        gl_quad_shader_program = LinkShader(
            quad_vertex_shader_string,
            quad_fragment_shader_string);

        gl_quad_projection_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "projection");

        gl_quad_view_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "view");

        gl_quad_model_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "model");

        gl_quad_viewport_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "viewport");

        gl_quad_texture_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "tex");

        gl_quad_sdf_function_location = glGetUniformLocation(
            gl_quad_shader_program,
            "sdf_func");

        gl_quad_brightness_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "brightness");

        gl_quad_texture_blend_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "tex_blend");

        gl_quad_gradient_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "gradient");

        gl_quad_gradient_0_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "gradient_0");

        gl_quad_gradient_1_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "gradient_1");

        gl_quad_alpha_margin_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "alpha_margin");

        gl_quad_nonlinearity_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "nonlinearity");

        gl_quad_texscale_uniform_location = glGetUniformLocation(
            gl_quad_shader_program,
            "tex_scale");

        glGenSamplers(
            1, &gl_quad_sampler_state);

        glActiveTexture(
            GL_TEXTURE0);

        glSamplerParameteri(
            gl_quad_sampler_state,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);

        glSamplerParameteri(
            gl_quad_sampler_state,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);

        glSamplerParameteri(
            gl_quad_sampler_state,
            GL_TEXTURE_MAG_FILTER,
            GL_NEAREST);

        glSamplerParameteri(
            gl_quad_sampler_state,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    GLInterfaceInstance::~GLInterfaceInstance()
    {
        glDeleteProgram(
            gl_quad_shader_program);

        glDeleteSamplers(
            1,
            &gl_quad_sampler_state);
    }

    void GLInterfaceInstance::Draw(RenderState state, DisplayNode* node)
    {
        if (node->Passthrough())
            return;

        if (node->Texture() != nullptr)
        {
            node->Texture()->Update();

            const auto gl_texture_handle = std::dynamic_pointer_cast<GLTextureHandle>(
                node->Texture())->gl_texture_handle;

            glActiveTexture(
                GL_TEXTURE0);

            glBindTexture(
                GL_TEXTURE_2D,
                gl_texture_handle);

            glUniform1i(
                gl_quad_texture_uniform_location,
                0);

            glBindSampler(
                0,
                gl_quad_sampler_state);

            if (node->maintain_pixel_scaling_horizontal)
            {
                const float x_scale = state.viewport.z / node->Texture()->width;
                node->tex_scale = vec2(x_scale, node->tex_scale.y);
            }
        }

        node->Validate();

        glUniform1i(
            gl_quad_sdf_function_location,
            static_cast<GLint>(node->sdf_func));

        glUniform2fv(
            gl_quad_texscale_uniform_location,
            1,
            &node->tex_scale[0]);

        glUniform1f(
            gl_quad_nonlinearity_uniform_location,
            node->nonlinearity->Value());

        glUniform1f(
            gl_quad_texture_blend_uniform_location,
            node->texture_blend->Value());

        glUniform1f(
            gl_quad_brightness_uniform_location,
            node->brightness->Value());

        glUniform1f(
            gl_quad_gradient_uniform_location,
            node->gradient->Value());

        glUniform1f(
            gl_quad_alpha_margin_uniform_location,
            node->alpha_margin->Value());

        glUniform3fv(
            gl_quad_gradient_0_uniform_location,
            1,
            &node->gradient_0->Value()[0]);

        glUniform3fv(
            gl_quad_gradient_1_uniform_location,
            1,
            &node->gradient_1->Value()[0]);

        const glm::mat4 transform = node->Transform();

        glUniformMatrix4fv(
            gl_quad_model_uniform_location,
            1,
            false,
            &transform[0][0]);

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(quad_indices->data->size()),
            GL_UNSIGNED_INT,
            static_cast<char const*>(0));
    }

    void GLInterfaceInstance::Bind(RenderState state)
    {
        glUseProgram(
            gl_quad_shader_program);

        glUniformMatrix4fv(
            gl_quad_projection_uniform_location,
            1,
            false,
            &state.projection[0][0]);

        glUniformMatrix4fv(
            gl_quad_view_uniform_location,
            1,
            false,
            &state.view[0][0]);

        glUniform4fv(
            gl_quad_viewport_uniform_location,
            1,
            &state.viewport[0]);

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

    void GLInterfaceInstance::Unbind()
    {
        glBindTexture(
            GL_TEXTURE_2D,
            NULL);
    }
}
}
}
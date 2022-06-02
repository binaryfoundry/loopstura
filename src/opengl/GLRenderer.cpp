#include "GLRenderer.hpp"

#include <stack>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

#include "GLTexture.hpp"
#include "GLStream.hpp"

#include "../properties/Property.hpp"

using namespace Application::Properties;

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    static std::initializer_list<float> quad_vertices_data
    {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f
    };

    static std::initializer_list<uint32_t> quad_indices_data
    {
         0, 1, 2, 2, 3, 0
    };

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
        uniform int sdf_func;
        uniform float brightness;
        uniform float gradient;
        uniform vec3 gradient_0;
        uniform vec3 gradient_1;
        uniform float outline_margin;
        float buff = 1.0;
        float alpha_margin = 1.0;
        float nonlinearity = 8.0;
        float sdCircle(in vec2 p, in float r) {
            return length(p) - r;
        }
        float sdBox(in vec2 p, in vec2 b)  {
            vec2 d = abs(p) - b;
            return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
        }
        float sdRoundedBox(in vec2 p, in vec2 b, in float r){
            vec2 q = abs(p)-b+r;
            return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r;
        }
        void main() {
            float d;
            if (sdf_func == 0) {
                d = sdBox(v_texcoord.xy - vec2(0.5), vec2(0.5));
            }
            else if (sdf_func == 1) {
                d = sdCircle(v_texcoord.xy - vec2(0.5), 0.5);
            }
            if (sdf_func == 2) {
                d = sdRoundedBox(v_texcoord.xy - vec2(0.5), vec2(0.5), 0.2);
            }
            float d2 = 1.0 - abs(d);
            float e = length(vec2(dFdx(d), dFdy(d)));
            float alpha_width = alpha_margin * e;
            float alpha = smoothstep(buff - alpha_width, buff, 1.0 - d);
            alpha = d > 0.0 ? alpha : 1.0; // clamp to outside SDF shape
            vec3 c = mix(linear(gradient_0), linear(gradient_1), 1.0 - pow(d2, nonlinearity));
            vec3 t =  linear(texture(tex, v_texcoord.xy)).xyz;
            c = mix(c, t, tex_blend);
            c = mix(c, vec3(1.0), clamp(brightness, 0.0, 1.0));
            c = mix(c, vec3(0.0), clamp(-brightness, 0.0, 1.0));
            out_color = vec4(gamma(c), alpha);
        })";

    GLRenderer::GLRenderer(
        ContextPtr context,
        std::function<void()> swap_buffers,
        uint32_t width,
        uint32_t height) :
        Renderer(
            context,
            swap_buffers,
            width,
            height),
        context(context)
    {
        GLImgui::Initialise();
        imgui = std::make_unique<GLImgui>(context);

        quad_vertices = std::make_unique<GLStream<float>>(
            StreamUsage::DYNAMIC,
            quad_vertices_data);

        quad_indices = std::make_unique<GLStream<uint32_t>>(
            StreamUsage::DYNAMIC,
            quad_indices_data);

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

        gl_quad_outline_margin_function_location = glGetUniformLocation(
            gl_quad_shader_program,
            "outline_margin");

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

    GLRenderer::~GLRenderer()
    {
        GLImgui::Destroy();

        glDeleteSamplers(
            1,
            &gl_quad_sampler_state);

        glDeleteProgram(
            gl_quad_shader_program);
    }

    TextureRGBA8Ptr GLRenderer::MakeTexture(
        uint32_t width,
        uint32_t height)
    {
        std::shared_ptr<std::vector<uint8_t>> data =
            std::make_shared<std::vector<uint8_t>>();

        data->resize(
            width * height * 4);

        return std::make_shared<GLTexture<TextureDataByte>>(
            width,
            height,
            data);
    }

    TextureRGBA8Ptr GLRenderer::MakeTexture(
        std::string file)
    {
        uint8_t bpp = 0;
        uint32_t tex_width = 0, tex_height = 0;

        std::shared_ptr<std::vector<uint8_t>> data =
            std::make_shared<std::vector<uint8_t>>();

        context->load_texture_2d(
            file, bpp, tex_width, tex_height, data);

        // TODO check matching BPP

        auto new_texture = std::make_shared<GLTexture<TextureDataByte>>(
            tex_width,
            tex_height,
            data);

        return new_texture;
    }

    void GLRenderer::Draw(RenderState state)
    {
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glViewport(
            static_cast<uint32_t>(state.viewport.x),
            static_cast<uint32_t>(state.viewport.y),
            static_cast<uint32_t>(state.viewport.z),
            static_cast<uint32_t>(state.viewport.w));

        glClearColor(
            1, 1, 1, 1);

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT);

        glEnable(
            GL_BLEND);

        glBlendFunc(
            GL_SRC_ALPHA,
            GL_ONE_MINUS_SRC_ALPHA);

        DrawNodes(state, root_node.get());

        imgui->Draw(state);

        swap_buffers();
    }

    void GLRenderer::DrawNode(DisplayNode* node)
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
        }

        node->Validate();

        glUniform1i(
            gl_quad_sdf_function_location,
            node->sdf_func);

        glUniform1f(
            gl_quad_texture_blend_uniform_location,
            node->texture_blend->Value());

        glUniform1f(
            gl_quad_outline_margin_function_location,
            node->outline_margin->Value());

        glUniform1f(
            gl_quad_brightness_uniform_location,
            node->brightness->Value());

        glUniform1f(
            gl_quad_gradient_uniform_location,
            node->gradient->Value());

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

    void GLRenderer::DrawNodes(RenderState state, DisplayNode* node)
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

        {
            std::unordered_set<DisplayNode*> visited;
            std::stack<DisplayNode*> stack;
            stack.push(root_node.get());

            while (!stack.empty())
            {
                DisplayNode* s = stack.top();
                stack.pop();

                if (visited.find(s) == visited.end())
                {
                    DrawNode(s);
                    visited.insert(s);
                }

                for (auto c : s->Children())
                {
                    if (visited.find(c) == visited.end())
                    {
                        stack.push(c);
                    }
                }
            }
        }

        glBindTexture(
            GL_TEXTURE_2D,
            NULL);
    }
}
}
}

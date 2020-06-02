#include "GLRenderer.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>

#include "GLTexture.hpp"

namespace Application
{
namespace OpenGL
{
    std::vector<GLfloat> quad_vertices =
    {
        -1.0f,  1.0f, 0.0f,
         1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f,  1.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f,
         1.0f,  1.0f, 0.0f,
         0.0f,  0.0f
    };

    std::vector<GLuint> quad_indices =
    {
         0, 1, 2, 2, 3, 0
    };

    std::string vertex_shader_string_basic_texture =
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
    
    std::string fragment_shader_string_basic_texture =
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

    GLRenderer::GLRenderer(
        uint32_t width,
        uint32_t height,
        std::function<void()> swap_buffers) :
        Application::Renderer(
            width,
            height),
        swap_buffers(swap_buffers)
    {
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        shader_program_basic = LinkShader(
            vertex_shader_string_basic_texture,
            fragment_shader_string_basic_texture);

        texture = MakeTexture(512, 512);

        quad_instance = std::make_shared<GLInstanceBasic>(
            shader_program_basic,
            quad_vertices,
            quad_indices,
            texture);
    }

    GLRenderer::~GLRenderer()
    {
    }

    std::shared_ptr<Texture<TextureDataByteRGBA>> GLRenderer::MakeTexture(
        uint32_t width,
        uint32_t height)
    {
        return std::make_shared<GLTexture<TextureDataByteRGBA>>(
            width, height);
    }

    void GLRenderer::Render()
    {
        glViewport(
            0,
            0,
            width,
            height);

        glClearColor(
            0, 1, 1, 1);

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT);

        quad_instance->Draw();

        swap_buffers();
    }

    void CheckError()
    {
        GLenum err = GL_NO_ERROR;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "gl error: " << err << std::endl;
        }

        if (err != GL_NO_ERROR)
        {
            throw std::runtime_error("gl error");
        }
    }
}
}

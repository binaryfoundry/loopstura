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

    template<typename T>
    GLuint GenBuffer(
        std::vector<T>& data,
        GLenum target,
        GLenum usage);

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

        quad_vertex_buffer = GenBuffer(
            quad_vertices,
            GL_ARRAY_BUFFER,
            GL_STATIC_DRAW);

        quad_index_buffer = GenBuffer(
            quad_indices,
            GL_ELEMENT_ARRAY_BUFFER,
            GL_STATIC_DRAW);

        texture = MakeTexture(512, 512);

        shader_program_basic = LinkShader(
            vertex_shader_string_basic_texture,
            fragment_shader_string_basic_texture);

        shader_basic = std::make_shared<GLMaterialBasic>(
            shader_program_basic,
            texture);
    }

    GLRenderer::~GLRenderer()
    {
        glDeleteBuffers(
            1, &quad_vertex_buffer);
        glDeleteBuffers(
            1, &quad_index_buffer);
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

        shader_basic->Bind();

        glBindBuffer(
            GL_ARRAY_BUFFER,
            quad_vertex_buffer);

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
            quad_index_buffer);

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(quad_indices.size()),
            GL_UNSIGNED_INT,
            static_cast<char const*>(0));

        shader_basic->Unbind();

        swap_buffers();
    }

    template<typename T>
    GLuint GenBuffer(
        std::vector<T>& data,
        GLenum target,
        GLenum usage)
    {
        GLuint buffer;

        glGenBuffers(
            1,
            &buffer);

        glBindBuffer(
            target,
            buffer);

        glBufferData(
            target,
            sizeof(T) * data.size(),
            &data[0],
            usage);

        glBindBuffer(
            target,
            NULL);

        return buffer;
    }

}
}

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

    GLuint LinkShader(
        std::string vertex_shader_string,
        std::string fragment_shader_string);

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

        shader = LinkShader(
            vertex_shader_string,
            fragment_shader_string);

        texture_uniform_location = glGetUniformLocation(
            shader,
            "tex");

        texture = MakeTexture(512, 512);

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

    GLRenderer::~GLRenderer()
    {
        glDeleteBuffers(
            1, &quad_vertex_buffer);
        glDeleteBuffers(
            1, &quad_index_buffer);

        glDeleteProgram(
            shader);

        glDeleteSamplers(
            1, &sampler_state);
    }

    std::shared_ptr<Texture<TexureDataByteRGBA>> GLRenderer::MakeTexture(
        uint32_t width,
        uint32_t height)
    {
        return std::make_shared<GLTexture<TexureDataByteRGBA>>(
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

        glUseProgram(
            shader);

        glActiveTexture(
            GL_TEXTURE0);

        texture->Bind();

        glBindSampler(
            0,
            sampler_state);

        glUniform1i(
            0,
            texture_uniform_location);

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

        texture->Unbind();

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

    GLuint LoadShader(
        GLenum type,
        const char *shader_src);

    GLuint LinkShader(
        std::string vertex_shader_string,
        std::string fragment_shader_string)
    {
        GLuint vertex_shader = LoadShader(
            GL_VERTEX_SHADER,
            vertex_shader_string.c_str());

        GLuint fragment_shader = LoadShader(
            GL_FRAGMENT_SHADER,
            fragment_shader_string.c_str());

        GLuint program_object = glCreateProgram();

        if (program_object == 0)
        {
            throw std::runtime_error(
                "shader init error");
        }

        glAttachShader(
            program_object,
            vertex_shader);

        glAttachShader(
            program_object,
            fragment_shader);

        glBindAttribLocation(
            program_object,
            0,
            "v_position");

        glLinkProgram(
            program_object);

        GLint linked;

        glGetProgramiv(
            program_object,
            GL_LINK_STATUS,
            &linked);

        if (!linked)
        {
            GLint info_len = 0;
            glGetProgramiv(
                program_object,
                GL_INFO_LOG_LENGTH,
                &info_len);

            if (info_len > 1)
            {
                char* info_log = new char[sizeof(char) * info_len];
                glGetProgramInfoLog(
                    program_object,
                    info_len,
                    NULL,
                    info_log);
                std::cout << "link error " << info_log << std::endl;
                free(info_log);
            }

            glDeleteProgram(
                program_object);
            throw std::runtime_error(
                "shader init error");
        }

        return program_object;
    }

    GLuint LoadShader(GLenum type, const char *shader_src)
    {
        GLuint shader;
        GLint compiled;

        shader = glCreateShader(type);
        if (shader == 0)
            return 0;

        glShaderSource(
            shader,
            1,
            &shader_src,
            NULL);

        glCompileShader(
            shader);

        glGetShaderiv(
            shader,
            GL_COMPILE_STATUS,
            &compiled);

        if (!compiled)
        {
            GLint info_len = 0;
            glGetShaderiv(
                shader,
                GL_INFO_LOG_LENGTH,
                &info_len);

            if (info_len > 1)
            {
                char* info_log = new char[sizeof(char) * info_len];
                glGetShaderInfoLog(
                    shader,
                    info_len,
                    NULL,
                    info_log);
                std::cout << "compile error " << info_log << std::endl;
            }

            glDeleteShader(
                shader);
            throw std::runtime_error(
                "shader init error");
        }

        return shader;
    }
}
}

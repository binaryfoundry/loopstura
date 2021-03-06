#include "GL.hpp"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
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
                delete[] info_log;
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
}

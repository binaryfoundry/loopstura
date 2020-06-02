#pragma once

#if defined (IS_PLATFORM_OSX)

#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#elif defined (IS_PLATFORM_WIN)

#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#include <GLES2/gl2ext.h>

#elif defined (IS_PLATFORM_EMSCRIPTEN)

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#endif

#include <iostream>

class GLBindable
{
public:
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
};

static void GLCheckError()
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

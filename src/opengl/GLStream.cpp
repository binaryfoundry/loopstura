#include "GLStream.hpp"

namespace Application
{
namespace Rendering
{
namespace OpenGL
{
    template<typename T>
    GLStream<T>::GLStream(
        StreamUsage usage,
        std::initializer_list<T>& list) :
        Stream<T>(list),
        gl_target(GL_ARRAY_BUFFER)
    {
        Initialise(usage);
    }

    template<>
    GLStream<float>::GLStream(
        StreamUsage usage,
        std::initializer_list<float>& list) :
        data(std::make_unique<std::vector<float>>(list)),
        gl_target(GL_ARRAY_BUFFER)
    {
        Initialise(usage);
    }

    template<>
    GLStream<uint32_t>::GLStream(
        StreamUsage usage,
        std::initializer_list<uint32_t>& list) :
        data(std::make_unique<std::vector<uint32_t>>(list)),
        gl_target(GL_ELEMENT_ARRAY_BUFFER)
    {
        Initialise(usage);
    }

    template<typename T>
    GLStream<T>::~GLStream()
    {
        glDeleteBuffers(
            1,
            &gl_buffer_handle);
    }

    template<typename T>
    void GLStream<T>::Initialise(
        StreamUsage usage)
    {
        switch (usage)
        {
            case StreamUsage::STATIC:
                gl_usage = GL_STATIC_DRAW;
                break;
            case StreamUsage::DYNAMIC:
                gl_usage = GL_DYNAMIC_DRAW;
                break;
        }

        glGenBuffers(
            1,
            &gl_buffer_handle);

        glBindBuffer(
            gl_target,
            gl_buffer_handle);

        glBufferData(
            gl_target,
            sizeof(T) * data->size(),
            &(*data)[0],
            gl_usage);

        glBindBuffer(
            gl_target,
            NULL);

        invalidated = false;
    }

    template <typename T>
    void GLStream<T>::Update()
    {
        if (invalidated)
        {
            glBindBuffer(
                gl_target,
                gl_buffer_handle);

            glBufferSubData(
                gl_target,
                0,
                sizeof(T) * data->size(),
                &(*data)[0]);

            glBindBuffer(
                gl_target,
                NULL);

           invalidated = false;
        }
    }

    template class GLStream<float>;
    template class GLStream<uint32_t>;
}
}
}

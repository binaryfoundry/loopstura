#include "GLStream.hpp"

namespace Application
{
namespace OpenGL
{
    template<>
    GLStream<float>::GLStream(
       std::initializer_list<float>& list) :
       Stream<float>(list),
       target(GL_ARRAY_BUFFER)
    {
        Initialise();
    }

    template<>
    GLStream<uint32_t>::GLStream(
        std::initializer_list<uint32_t>& list) :
        Stream<uint32_t>(list),
        target(GL_ELEMENT_ARRAY_BUFFER)
    {
        Initialise();
    }

    template<typename T>
    GLStream<T>::~GLStream()
    {
        glDeleteBuffers(
            1,
            &gl_buffer_handle);
    }

    template<typename T>
    void GLStream<T>::Initialise()
    {
        glGenBuffers(
            1,
            &gl_buffer_handle);

        glBindBuffer(
            target,
            gl_buffer_handle);

        glBufferData(
            target,
            sizeof(T) * data->size(),
            &(*data)[0],
            usage);

        glBindBuffer(
            target,
            NULL);
    }

    template <typename T>
    void GLStream<T>::Update()
    {
        if (Stream<T>::invalidated)
        {
            glBindBuffer(
                target,
                gl_buffer_handle);

            glBufferData(
                target,
                sizeof(T) * data->size(),
                &(*data)[0],
                usage);

            glBindBuffer(
                target,
                NULL);

            Stream<T>::invalidated = false;
        }
    }
}
}

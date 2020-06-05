#include "Renderer.hpp"

namespace Application
{
    Renderer::Renderer(
        ContextPtr context,
        std::function<void()> swap_buffers,
        uint32_t new_width,
        uint32_t new_height) :
        context(context),
        swap_buffers(swap_buffers)
    {
        Resize(new_width, new_height);
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Resize(
        uint32_t new_width,
        uint32_t new_height)
    {
        width = new_width;
        height = new_height;

        state.projection = glm::ortho(
            0,
            static_cast<int>(width),
            0,
            static_cast<int>(height),
            -1000,
            1000);

        state.viewport = vec4(
            0, 0, width, height);
    }
}

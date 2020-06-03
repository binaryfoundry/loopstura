#include "Renderer.hpp"

namespace Application
{
    Renderer::Renderer(
        ContextPtr context,
        std::function<void()> swap_buffers,
        uint32_t width,
        uint32_t height) :
        context(context),
        swap_buffers(swap_buffers),
        width(width),
        height(height)
    {
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Resize(uint32_t new_width, uint32_t new_height)
    {
        width = new_width;
        height = new_height;
    }
}

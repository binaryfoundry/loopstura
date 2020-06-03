#include "Renderer.hpp"

namespace Application
{
    Renderer::Renderer(
        uint32_t width,
        uint32_t height,
        std::function<void()> swap_buffers) :
        width(width),
        height(height),
        swap_buffers(swap_buffers)
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

    void Renderer::Draw()
    {
        Begin();

        quad_instance->Draw();

        End();
    }
}

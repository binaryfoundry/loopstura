#include "Renderer.hpp"

namespace Application
{
    Renderer::Renderer(
        uint32_t width,
        uint32_t height) :
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

    void Renderer::Draw()
    {
        Begin();

        quad_instance->Draw();

        End();
    }
}

#include "Renderer.hpp"

namespace Application
{
    Renderer::Renderer(
        uint32_t width,
        uint32_t height) :
        width(width),
        height(height)
    {
        texture_data.resize(
            texture_width * texture_height * 4);
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::SetSize(uint32_t new_width, uint32_t new_height)
    {
        width = new_width;
        height = new_height;
    }
}

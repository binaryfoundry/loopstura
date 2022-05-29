#include "Renderer.hpp"

namespace Application
{
namespace Rendering
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

        state.projection = glm::ortho<float>(
            0,
            static_cast<float>(width),
            static_cast<float>(height),
            0,
           -1.0f,
            1.0f);

        state.viewport = vec4(
            0, 0, width, height);
    }


    void Renderer::RegisterNode(DisplayNodePtr quad)
    {
        nodes.push_back(quad);
    }
}
}

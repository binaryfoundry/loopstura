#include "Renderer.hpp"

namespace Application
{
    Renderer::Renderer(
        ContextPtr context,
        std::function<void()> swap_buffers,
        uint32_t new_width,
        uint32_t new_height) :
        context(context),
        swap_buffers(swap_buffers),
        view(std::make_shared<Property<glm::mat4>>(mat4())),
        projection(std::make_shared<Property<glm::mat4>>(mat4())),
        viewport(std::make_shared<Property<glm::vec4>>(vec4(0, 0, 1, 1)))
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

        projection->Set(glm::ortho(
            0,
            static_cast<int>(width),
            0,
            static_cast<int>(height),
            -1000,
            1000));
    }
}

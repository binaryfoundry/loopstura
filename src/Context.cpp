#include "Context.hpp"

namespace Application
{
    Context::Context(
        Texure2DLoadCallback load_texture_2d) :
        load_texture_2d(load_texture_2d),
        property_manager(std::make_unique<Properties::Manager>())
    {
    }

    Context::~Context()
    {
    }

    void Context::Update()
    {
        property_manager->Update();
    }
}

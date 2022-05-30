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

    void Context::Update(const float timestep)
    {
        property_manager->Update(timestep);
    }
}

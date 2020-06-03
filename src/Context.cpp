#include "Context.hpp"

namespace Application
{
    Context::Context() :
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
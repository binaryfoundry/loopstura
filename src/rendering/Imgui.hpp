#pragma once

#include <memory>

#include "../rendering/Instance.hpp"
#include "../rendering/Texture.hpp"
#include "../Context.hpp"

#include "../properties/Property.hpp"

using namespace Application::Properties;

namespace Application
{
namespace Rendering
{
    class Imgui : public Instance
    {
    protected:
        Imgui(
            ContextPtr context);

    public:

        virtual ~Imgui();
    };
}
}

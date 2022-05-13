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
    class InstanceImgui : public Instance
    {
    protected:
        InstanceImgui(
            ContextPtr context);

    public:

        virtual ~InstanceImgui();
    };
}
}

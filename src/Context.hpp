#pragma once

#include "properties/Manager.hpp"

#include <memory>

namespace Application
{
class Context
{
    class Client;
    friend class Client;

public:
    Context();

public:
    ~Context();

    const std::shared_ptr<Properties::Manager> property_manager;

    void Update();
};
}

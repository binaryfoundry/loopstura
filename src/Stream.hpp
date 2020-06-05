#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
    using VerticesPtr = std::shared_ptr<std::vector<float>>;
    using IndicesPtr = std::shared_ptr<std::vector<uint32_t>>;
}

#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
    template <typename T>
    class Stream
    {
    public:
        Stream(std::initializer_list<T>& list) :
            data(std::make_shared<std::vector<T>>(list))
        {
        }

        const std::shared_ptr<std::vector<T>> data;

        bool invalidated = true;

        void Invalidate()
        {
            invalidated = true;
        }
    };

    using VertexStream = Stream<float>;
    using IndexStream = Stream<uint32_t>;

    using VertexStreamPtr = std::shared_ptr<VertexStream>;
    using IndexStreamPtr = std::shared_ptr<IndexStream>;
}

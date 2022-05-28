#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

namespace Application
{
namespace Rendering
{
    enum class StreamUsage
    {
        STATIC,
        DYNAMIC
    };

    template <typename T>
    class Stream
    {
    protected:
        Stream(std::initializer_list<T>& list) :
            data(std::make_shared<std::vector<T>>(list))
        {
        }

    public:
        const std::shared_ptr<std::vector<T>> data;

        bool invalidated = true;

        void Invalidate()
        {
            invalidated = true;
        }

        virtual void Update() = 0;
    };

    using VertexStream = Stream<float>;
    using IndexStream = Stream<uint32_t>;

    using VertexStreamPtr = std::shared_ptr<VertexStream>;
    using IndexStreamPtr = std::shared_ptr<IndexStream>;
}
}

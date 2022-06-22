#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <functional>

#include "../Math.hpp"
#include "Easing.hpp"

namespace Application
{
namespace Properties
{
    using PropertyAnimationCallback = std::function<void(std::shared_ptr<void>)>;
    using PropertyTimeoutCallback = std::function<void()>;

    template <class T> class Property
    {
        friend class PropertyAnimator;
        template <class U> friend class Animator;

    private:
        T local_value;

    protected:
        T* value_ptr;
        T* target_value;
        T origin_value;

        std::function<void()> invalidate = [&]() {};

    public:
        Property(T value_)
        {
            local_value = value_;
            value_ptr = &local_value;
        }

        Property(T* value_)
        {
            value_ptr = value_;
            local_value = *value_ptr;
        }

        Property(T value_, std::function<void()> invalidate) :
            invalidate(invalidate)
        {
            local_value = value_;
            value_ptr = &local_value;
        }

        Property(T* value_, std::function<void()> invalidate) :
            invalidate(invalidate)
        {
            value_ptr = value_;
        }

        T Value()
        {
            return *value_ptr;
        }

        void Set(T value)
        {
            (*value_ptr) = value; invalidate();
        }

        void* Get()
        {
            return value_ptr;
        }

        void operator =(const T rhs) { (*value_ptr) = rhs; invalidate(); }
        void operator+=(const T rhs) { (*value_ptr) += rhs; invalidate(); }
        void operator*=(const T rhs) { (*value_ptr) *= rhs; invalidate(); }
        void operator/=(const T rhs) { (*value_ptr) /= rhs; invalidate(); }
    };

    template<class T>
    using PropertyPtr = std::shared_ptr<Property<T>>;
}
}

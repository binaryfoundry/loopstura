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

        bool local_dirty_flag = true;
        bool* dirty_flag;

        virtual void Invalidate()
        {
            *dirty_flag = true;
        }

    public:
        Property(T* value_)
        {
            dirty_flag = &local_dirty_flag;
            value_ptr = value_;
            local_value = *value_ptr;
        }

        Property(bool* dirty_flag) :
            dirty_flag(dirty_flag)
        {
            value_ptr = &local_value;
        }

        T Value()
        {
            return *value_ptr;
        }

        void Set(T value)
        {
            (*value_ptr) = value; Invalidate();
        }

        void* Get()
        {
            return value_ptr;
        }

        void operator =(const T&& rhs) { (*value_ptr) = rhs; Invalidate(); }
        void operator+=(const T&& rhs) { (*value_ptr) += rhs; Invalidate(); }
        void operator*=(const T&& rhs) { (*value_ptr) *= rhs; Invalidate(); }
        void operator/=(const T&& rhs) { (*value_ptr) /= rhs; Invalidate(); }
    };

    template <class T> struct PropertyAnimation
    {
        std::shared_ptr<Property<T>> prop;
        EasingFunction easing_function;
        PropertyAnimationCallback callback;
        float step;
        float current_time;
    };

    struct TimeOut
    {
        float current_time;
        float step;
        PropertyTimeoutCallback callback;
    };
}
}
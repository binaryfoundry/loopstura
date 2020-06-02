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
        template <class U> friend class GenericAnimator;

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

    template <class T> class GenericAnimator
    {
    private:
        T fixed_target;

        std::vector<PropertyAnimation<T>> ease;
        std::vector<PropertyAnimation<T>> ready_callbacks;

        template<class A> A Interpolate(const A origin, const A target, const float t)
        {
            return origin + ((target - origin) * t);
        }

        quat Interpolate(quat origin, quat target, float t)
        {
            return glm::slerp(origin, target, t);
        }

    public:
        void AddTween(
            std::shared_ptr<Property<T>> prop,
            float duration,
            T origin,
            T* target,
            EasingFunction easing_function,
            PropertyAnimationCallback callback)
        {
            float step = (1.0f / 60.0f) / duration;

            prop->target_value = target;
            prop->origin_value = origin;
            ease.push_back({
                prop,
                easing_function,
                callback,
                step,
                0 });
        }

        void AddTween(
            std::shared_ptr<Property<T>> prop,
            float duration,
            T origin,
            T target,
            EasingFunction easing_function,
            PropertyAnimationCallback callback)
        {
            fixed_target = target;
            AddTween(
                prop,
                duration,
                origin,
                &fixed_target,
                easing_function,
                callback);
        }

        void Update()
        {
            auto i = ease.begin();
            while (i != ease.end())
            {
                if (i->current_time >= 1.0f)
                {
                    i->prop->Set(*i->prop->target_value);
                    ready_callbacks.push_back(*i);
                    i = ease.erase(i);
                }
                else
                {
                    T origin = i->prop->origin_value;
                    T target = *i->prop->target_value;
                    float t = Ease(i->easing_function, i->current_time);
                    i->prop->Set(Interpolate(origin, target, t));
                    i->current_time += i->step;
                    ++i;
                }
            }

            for (auto it = ready_callbacks.begin(); it != ready_callbacks.end(); ++it)
            {
                std::shared_ptr<void> data = std::static_pointer_cast<void>((*it).prop);
                (*it).callback(data);
            }

            ready_callbacks.clear();
        }
    };
}
}

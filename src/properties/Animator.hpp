#pragma once

#include <vector>
#include <memory>

#include "Property.hpp"

namespace Application
{
namespace Properties
{
    template <class T> struct PropertyAnimation
    {
        std::shared_ptr<Property<T>> prop;
        EasingFunction easing_function;
        PropertyAnimationCallback callback;
        float duration;
        float current_time;
    };

    struct TimeOut
    {
        float duration;
        float current_time;
        PropertyTimeoutCallback callback;
    };

    template <typename T>
    class Animator
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
        Animator() = default;
        Animator(const Animator&) = delete;

        void AddTween(
            std::shared_ptr<Property<T>> prop,
            float duration,
            T origin,
            T* target,
            EasingFunction easing_function,
            PropertyAnimationCallback callback)
        {
            prop->target_value = target;
            prop->origin_value = origin;
            ease.push_back({
                prop,
                easing_function,
                callback,
                duration,
                0
            });
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

        void Update(const float timestep)
        {
            auto i = ease.begin();
            while (i != ease.end())
            {
                if (i->current_time >= i->duration)
                {
                    i->prop->Set(*i->prop->target_value);
                    ready_callbacks.push_back(*i);
                    i = ease.erase(i);
                }
                else
                {
                    T origin = i->prop->origin_value;
                    T target = *i->prop->target_value;
                    float t = Ease(i->easing_function, i->current_time / i->duration);
                    i->prop->Set(Interpolate(origin, target, t));
                    i->current_time += timestep;
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
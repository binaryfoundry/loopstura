#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <functional>

#include "Math.hpp"

namespace Application
{
    using PropertyAnimationCallback = std::function<void(std::shared_ptr<void>)>;
    using PropertyTimeoutCallback = std::function<void()>;

    enum class EasingFunction
    {
        Linear,
        EaseInQuad,
        EaseOutQuad,
        EaseInOutQuad,
        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic,
        EaseInQuart,
        EaseOutQuart,
        EaseInOutQuart,
        EaseInQuint,
        EaseOutQuint,
        EaseInOutQuint
    };

    inline float CalculateEasing(EasingFunction func, float t)
    {
        float k = t - 1;
        switch (func)
        {
        case EasingFunction::Linear:
            return t;
        case EasingFunction::EaseInQuad:
            return t * t;
        case EasingFunction::EaseOutQuad:
            return t * (2 - t);
        case EasingFunction::EaseInOutQuad:
            return t < .5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
        case EasingFunction::EaseInCubic:
            return t * t * t;
        case EasingFunction::EaseOutCubic:
            return k * k * k + 1;
        case EasingFunction::EaseInOutCubic:
            return t < .5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
        case EasingFunction::EaseInQuart:
            return t * t * t * t;
        case EasingFunction::EaseOutQuart:
            return 1 - k * k * k * k;
        case EasingFunction::EaseInOutQuart:
            return t < .5 ? 8 * t * t * t * t : 1 - 8 * k * k * k * k;
        case EasingFunction::EaseInQuint:
            return t * t * t * t * t;
        case EasingFunction::EaseOutQuint:
            return 1 + k * k * k * t * t;
        case EasingFunction::EaseInOutQuint:
            return t < .5 ? 16 * t * t * t *  t * t : 1 + 16 * k * k * k * k * k;
        }
        return 0;
    }

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

        void operator =(const T&& rhs) { (*value_ptr)  = rhs; Invalidate(); }
        void operator+=(const T&& rhs) { (*value_ptr) += rhs; Invalidate(); }
        void operator*=(const T&& rhs) { (*value_ptr) *= rhs; Invalidate(); }
        void operator/=(const T&& rhs) { (*value_ptr) /= rhs; Invalidate(); }
    };

    template <class T> struct PropertyAnimation
    {
        shared_ptr<Property<T>> prop;
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

        vector<PropertyAnimation<T>> ease;
        vector<PropertyAnimation<T>> ready_callbacks;

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
            shared_ptr<Property<T>> prop,
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
                0});
        }

        void AddTween(
            shared_ptr<Property<T>> prop,
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
                    float t = CalculateEasing(i->easing_function, i->current_time);
                    i->prop->Set(Interpolate(origin, target, t));
                    i->current_time += i->step;
                    ++i;
                }
            }

            for (auto it = ready_callbacks.begin(); it != ready_callbacks.end(); ++it)
            {
                shared_ptr<void> data = std::static_pointer_cast<void>((*it).prop);
                (*it).callback(data);
            }

            ready_callbacks.clear();
        }
    };

    class PropertyManager
    {
    private:
        std::vector<TimeOut> timeouts;
        std::shared_ptr<GenericAnimator<float>> float_animator;
        std::shared_ptr<GenericAnimator<vec2>>  vec2_animator;
        std::shared_ptr<GenericAnimator<vec3>>  vec3_animator;
        std::shared_ptr<GenericAnimator<vec4>>  vec4_animator;
        std::shared_ptr<GenericAnimator<quat>>  quat_animator;

    public:
        PropertyManager();

        void Update();

        void AddTimeOut(
            float duration,
            PropertyTimeoutCallback callback);

        void AddTween(
            std::shared_ptr<Property<float>> prop,
            float* target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});

        void AddTween(
            std::shared_ptr<Property<float>> prop,
            float target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});

        void AddTween(
            std::shared_ptr<Property<vec2>> prop,
            vec2* target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});

        void AddTween(
            std::shared_ptr<Property<vec2>> prop,
            vec2 target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});

        void AddTween(
            std::shared_ptr<Property<vec3>> prop,
            vec3* target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});

        void AddTween(
            std::shared_ptr<Property<vec3>> prop,
            vec3 target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});

        void AddTween(
            std::shared_ptr<Property<vec4>> prop,
            vec4* target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});

        void AddTween(
            std::shared_ptr<Property<vec4>> prop,
            vec4 target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});

        void AddTween(
            std::shared_ptr<Property<quat>> prop,
            quat* target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});

        void AddTween(
            std::shared_ptr<Property<quat>> prop,
            quat target,
            float duration_seconds,
            EasingFunction easing_function,
            PropertyAnimationCallback callback = [](std::shared_ptr<void> data) {});
    };
}

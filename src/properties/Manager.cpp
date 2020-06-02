#include "Manager.hpp"

namespace Application
{
namespace Properties
{
    Manager::Manager()
    {
        float_animator = std::make_shared<Animator<float>>();
        vec2_animator = std::make_shared<Animator<vec2>>();
        vec3_animator = std::make_shared<Animator<vec3>>();
        vec4_animator = std::make_shared<Animator<vec4>>();
        quat_animator = std::make_shared<Animator<quat>>();
    }

    void Manager::AddTimeOut(
        float duration,
        PropertyTimeoutCallback callback)
    {
        float step = (1.0f / 60.0f) / duration;

        timeouts.push_back({
            0,
            step,
            callback
            });
    }

    void Manager::AddTween(
        std::shared_ptr<Property<float>> prop,
        float* target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        float_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::AddTween(
        std::shared_ptr<Property<float>> prop,
        float target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        float_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::AddTween(
        std::shared_ptr<Property<vec2>> prop,
        vec2* target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        vec2_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::AddTween(
        std::shared_ptr<Property<vec2>> prop,
        vec2 target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        vec2_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::AddTween(
        std::shared_ptr<Property<vec3>> prop,
        vec3* target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        vec3_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::AddTween(
        std::shared_ptr<Property<vec3>> prop,
        vec3 target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        vec3_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::AddTween(
        std::shared_ptr<Property<vec4>> prop,
        vec4* target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        vec4_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::AddTween(
        std::shared_ptr<Property<vec4>> prop,
        vec4 target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        vec4_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::AddTween(
        std::shared_ptr<Property<quat>> prop,
        quat* target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        quat_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::AddTween(
        std::shared_ptr<Property<quat>> prop,
        quat target,
        float duration_seconds,
        EasingFunction easing_function,
        PropertyAnimationCallback callback)
    {
        quat_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void Manager::Update()
    {
        auto i = timeouts.begin();
        while (i != timeouts.end())
        {
            if (i->current_time >= 1.0f)
            {
                i->callback();
                i = timeouts.erase(i);
            }
            else
            {
                i->current_time += i->step;
                ++i;
            }
        }

        float_animator->Update();
        vec2_animator->Update();
        vec3_animator->Update();
        vec4_animator->Update();
        quat_animator->Update();
    }
}
}

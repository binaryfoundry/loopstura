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
        timeouts.push_back({
            duration,
            0,
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

    void Manager::Update(const float timestep)
    {
        auto i = timeouts.begin();
        while (i != timeouts.end())
        {
            if (i->current_time >= i->duration)
            {
                i->callback();
                i = timeouts.erase(i);
            }
            else
            {
                i->current_time += timestep;
                ++i;
            }
        }

        float_animator->Update(timestep);
        vec2_animator->Update(timestep);
        vec3_animator->Update(timestep);
        vec4_animator->Update(timestep);
        quat_animator->Update(timestep);
    }
}
}

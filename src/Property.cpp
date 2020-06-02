#include "Property.hpp"

namespace Application
{
    PropertyManager::PropertyManager()
    {
        float_animator = std::make_shared<GenericAnimator<float>>();
        vec2_animator = std::make_shared<GenericAnimator<vec2>>();
        vec3_animator = std::make_shared<GenericAnimator<vec3>>();
        vec4_animator = std::make_shared<GenericAnimator<vec4>>();
        quat_animator = std::make_shared<GenericAnimator<quat>>();
    }

    void PropertyManager::AddTimeOut(
        float duration,
        TimeOutCallback callback)
    {
        float step = (1.0f / 60.0f) / duration;

        timeouts.push_back({
            0,
            step,
            callback
        });
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<float>> prop,
        float* target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        float_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<float>> prop,
        float target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        float_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<vec2>> prop,
        vec2* target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        vec2_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<vec2>> prop,
        vec2 target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        vec2_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<vec3>> prop,
        vec3* target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        vec3_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<vec3>> prop,
        vec3 target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        vec3_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<vec4>> prop,
        vec4* target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        vec4_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<vec4>> prop,
        vec4 target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        vec4_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<quat>> prop,
        quat* target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        quat_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::AddTween(
        std::shared_ptr<Property<quat>> prop,
        quat target,
        float duration_seconds,
        EasingFunction easing_function,
        AnimCallback callback = [](std::shared_ptr<void> data) {})
    {
        quat_animator->AddTween(
            prop, duration_seconds, prop->Value(), target, easing_function, callback);
    }

    void PropertyManager::Update()
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

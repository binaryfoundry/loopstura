#pragma once

#include "Property.hpp"

#include <memory>
#include <vector>
#include <functional>

namespace Application
{
namespace Properties
{
    class Manager
    {
    private:
        std::vector<TimeOut> timeouts;
        std::shared_ptr<GenericAnimator<float>> float_animator;
        std::shared_ptr<GenericAnimator<vec2>>  vec2_animator;
        std::shared_ptr<GenericAnimator<vec3>>  vec3_animator;
        std::shared_ptr<GenericAnimator<vec4>>  vec4_animator;
        std::shared_ptr<GenericAnimator<quat>>  quat_animator;

    public:
        Manager();

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
}

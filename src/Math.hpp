#pragma once

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../../lib/glm/glm/gtc/quaternion.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;

// http://paulbourke.net/miscellaneous/interpolation/

double CosineInterpolate(
    double y1, double y2,
    double mu)
{
    double mu2;

    mu2 = (1 - cos(mu * std::numbers::pi)) / 2;
    return(y1 * (1 - mu2) + y2 * mu2);
}

double CubicInterpolate(
    double y0, double y1,
    double y2, double y3,
    double mu)
{
    double a0, a1, a2, a3, mu2;

    mu2 = mu * mu;
    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;

    return(a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
}

double CatmullRomInterpolate(
    double y0, double y1,
    double y2, double y3,
    double mu)
{
    double a0, a1, a2, a3, mu2;

    mu2 = mu * mu;
    a0 = -0.5 * y0 + 1.5 * y1 - 1.5 * y2 + 0.5 * y3;
    a1 = y0 - 2.5 * y1 + 2 * y2 - 0.5 * y3;
    a2 = -0.5 * y0 + 0.5 * y2;
    a3 = y1;

    return(a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
}

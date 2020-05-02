#pragma once

#include "engine/animation/track.hpp"

namespace ay::anim
{

static glm::vec3 inter_vec(glm::vec3 a, glm::vec3 b, float factor)
{
    return a * (1.0f - factor) + b * factor;
}

using Vec3Track = anim::Track<glm::vec3, decltype(inter_vec)>;
template<>
Vec3Track::fun_type Vec3Track::fun = &inter_vec;

}  // namespace ay::anim

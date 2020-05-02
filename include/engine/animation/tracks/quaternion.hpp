#pragma once

#include "engine/animation/track.hpp"

namespace ay::anim
{

static glm::quat inter_quat(glm::quat a, glm::quat b, float factor)
{
    return glm::slerp(a, b, factor);
}

using QuatTrack = anim::Track<glm::quat, decltype(inter_quat)>;
template<>
QuatTrack::fun_type QuatTrack::fun = &inter_quat;

}  // namespace ay::anim

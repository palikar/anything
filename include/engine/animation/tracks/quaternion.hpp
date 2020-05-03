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


QuatTrack::ptr_type vector_track(std::vector<glm::quat> quats, std::vector<float> times)
{
    auto p = std::make_unique<QuatTrack>(std::move(quats), std::move(times));
    return p;
}


}  // namespace ay::anim

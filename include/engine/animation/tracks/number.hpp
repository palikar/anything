#pragma once

#include "engine/animation/track.hpp"

namespace ay::anim
{
static float inter_num(float a, float b, float factor)
{

    return a * (1.0f - factor) + b * factor;
}

using NumTrack = anim::Track<float, decltype(inter_num)>;
template<>
NumTrack::fun_type NumTrack::fun = &inter_num;


NumTrack::ptr_type vector_track(std::vector<float> nums, std::vector<float> times)
{
    auto p = std::make_unique<NumTrack>(std::move(nums), std::move(times));
    return p;
}


}  // namespace ay::anim

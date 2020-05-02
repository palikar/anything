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


}  // namespace ay::anim

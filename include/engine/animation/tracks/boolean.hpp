#pragma once

#include "engine/animation/track.hpp"

namespace ay::anim
{

static bool inter_bool(bool a, bool b, float)
{
    return a && b;
}

using BoolTrack = anim::Track<bool, decltype(inter_bool)>;
template<>
BoolTrack::fun_type BoolTrack::fun = &inter_bool;

}  // namespace ay::anim

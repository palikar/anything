#pragma once

#include "engine/animation/track.hpp"

namespace ay::anim
{

static glm::vec3 inter_vec3(glm::vec3 a, glm::vec3 b, float factor)
{
    return a * (1.0f - factor) + b * factor;
}

static glm::vec2 inter_vec2(glm::vec2 a, glm::vec2 b, float factor)
{
    return a * (1.0f - factor) + b * factor;
}

static glm::vec4 inter_vec4(glm::vec4 a, glm::vec4 b, float factor)
{
    return a * (1.0f - factor) + b * factor;
}


using Vec4Track = anim::Track<glm::vec4, decltype(inter_vec4)>;
template<>
Vec4Track::fun_type Vec4Track::fun = &inter_vec4;

using Vec3Track = anim::Track<glm::vec3, decltype(inter_vec3)>;
template<>
Vec3Track::fun_type Vec3Track::fun = &inter_vec3;

using Vec2Track = anim::Track<glm::vec2, decltype(inter_vec2)>;
template<>
Vec2Track::fun_type Vec2Track::fun = &inter_vec2;


Vec4Track::ptr_type vector_track(std::vector<glm::vec4> vectors, std::vector<float> times)
{
    auto p = std::make_unique<Vec4Track>(std::move(vectors), std::move(times));
    return p;
}

Vec3Track::ptr_type vector_track(std::vector<glm::vec3> vectors, std::vector<float> times)
{
    auto p = std::make_unique<Vec3Track>(std::move(vectors), std::move(times));
    return p;
}

Vec2Track::ptr_type vector_track(std::vector<glm::vec2> vectors, std::vector<float> times)
{
    auto p = std::make_unique<Vec2Track>(std::move(vectors), std::move(times));
    return p;
}


}  // namespace ay::anim

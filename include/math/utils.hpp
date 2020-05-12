#pragma once

#include "util/random.hpp"
#include "glm_header.hpp"

namespace ay::mth
{

static constexpr float PI = 3.14156592;

inline glm::mat4 init_rotation(glm::vec3 forward, glm::vec3 up, glm::vec3 right)
{
    glm::mat4 m;

    m[0][0] = right.x;
    m[0][1] = right.y;
    m[0][2] = right.z;
    m[0][3] = 0;
    m[1][0] = up.x;
    m[1][1] = up.y;
    m[1][2] = up.z;
    m[1][3] = 0;
    m[2][0] = forward.x;
    m[2][1] = forward.y;
    m[2][2] = forward.z;
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;

    return m;
}

inline glm::mat4 init_rotation(glm::vec3 forward, glm::vec3 up)
{
    auto f = glm::normalize(forward);
    auto r = glm::cross(f, glm::normalize(up));
    auto u = glm::cross(f, r);
    return init_rotation(f, u, r);
}

inline glm::mat4 init_scale(float x, float y, float z)
{

    return glm::mat4(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
}

inline glm::mat4 init_scale(glm::vec3 v)
{
    return init_scale(v.x, v.y, v.z);
}

inline glm::mat4 init_translate(float x, float y, float z)
{

    return glm::mat4(1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1);
}

inline glm::mat4 init_translate(glm::vec3 v)
{
    return init_translate(v.x, v.y, v.z);
}

inline glm::mat4 init_identity()
{

    return glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

inline glm::vec3 rotate(glm::vec3 v, glm::quat q)
{
    // return glm::conjugate(q) * v * q;

    auto res = (q * glm::quat(0, v.x, v.y, v.z)) * glm::conjugate(q);
    return glm::vec3(res.x, res.y, res.z);
}

inline glm::vec3 up(glm::quat q)
{
    return rotate(glm::vec3(0, 1, 0), q);
}

inline glm::vec3 down(glm::quat q)
{
    return rotate(glm::vec3(0, -1, 0), q);
}

inline glm::vec3 right(glm::quat q)
{
    return rotate(glm::vec3(1, 0, 0), q);
}

inline glm::vec3 left(glm::quat q)
{
    return rotate(glm::vec3(-1, 0, 0), q);
}

inline glm::vec3 forward(glm::quat q)
{
    return glm::axis(q);
}

inline glm::vec3 backward(glm::quat q)
{
    return -glm::axis(q);
}

inline glm::vec3 to_vec3(glm::quat q)
{
    glm::vec3 v;

    v.x = 2 * (q.x * q.z - q.w * q.y);
    v.y = 2 * (q.y * q.z + q.w * q.x);
    v.z = 1 - 2 * (q.x * q.x + q.y * q.y);

    return v;
}

template<typename T>
inline T clamp(T x, T down, T up)
{
    if (x < down)
        return down;
    if (x > up)
        return up;
    return x;
}

template<typename T>
inline T saturate(T x)
{
    if (x > 1.0)
    {
        return 1.0;
    }

    if (x < 0.0)
    {
        return 0.0;
    }

    return x;
}

inline glm::mat3 normal(glm::mat4 mat)
{
    return glm::mat3(mat[0][0],
                     mat[0][1],
                     mat[0][2],
                     mat[1][0],
                     mat[1][1],
                     mat[1][2],
                     mat[2][0],
                     mat[2][1],
                     mat[2][2]);
}

inline float real_with_error(float base, float lower, float upper)
{
    return base + util::Random::uniform_real(lower, upper);
}

inline glm::vec2 vec2_with_error(glm::vec2 base, glm::vec2 lower, glm::vec2 upper)
{
    float x = util::Random::uniform_int(lower.x, upper.x);
    float y = util::Random::uniform_int(lower.y, upper.y);
    return base + glm::vec2(x, y);
}

inline glm::vec3 vec3_with_error(glm::vec3 base, glm::vec3 lower, glm::vec3 upper)
{
    float x = util::Random::uniform_int(lower.x, upper.x);
    float y = util::Random::uniform_int(lower.y, upper.y);
    float z = util::Random::uniform_int(lower.z, upper.z);

    return base + glm::vec3(x, y, z);
}

inline glm::vec4 vec4_with_error(glm::vec4 base, glm::vec4 lower, glm::vec4 upper)
{
    float x = util::Random::uniform_int(lower.x, upper.x);
    float y = util::Random::uniform_int(lower.y, upper.y);
    float z = util::Random::uniform_int(lower.z, upper.z);
    float w = util::Random::uniform_int(lower.w, upper.w);

    return base + glm::vec4(x, y, z, w);
}


inline float real_with_error(float base, float bounds[2])
{
    return real_with_error(base, bounds[0], bounds[1]);
}

inline glm::vec2 vec2_with_error(glm::vec2 base, glm::vec2 bounds[2])
{
    return vec2_with_error(base, bounds[0], bounds[1]);
}

inline glm::vec3 vec3_with_error(glm::vec3 base, glm::vec3 bounds[2])
{
    return vec3_with_error(base, bounds[0], bounds[1]);
}

inline glm::vec4 vec4_with_error(glm::vec4 base, glm::vec4 bounds[2])
{
    return vec4_with_error(base, bounds[0], bounds[1]);
}

}  // namespace ay::mth

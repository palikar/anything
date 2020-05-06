#pragma once

#include "util/gl_helpers.hpp"

#include "glm_header.hpp"

namespace ay::rend
{

inline constexpr const char *SHADER_HEADER = "#version 460 core";

enum class BlenidngEquation : GLenum
{
    ADD         = GL_FUNC_ADD,
    SUB         = GL_FUNC_SUBTRACT,
    REVERSE_SUB = GL_FUNC_REVERSE_SUBTRACT,
    MIN         = GL_MIN,
    MAX         = GL_MAX
};

enum class BlenidngSource : GLenum
{
    ZERO                = GL_ZERO,
    ONE                 = GL_ONE,
    SRC_COLOR           = GL_SRC_COLOR,
    ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
    SRC_ALPHA           = GL_SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
    DST_COLOR           = GL_DST_COLOR,
    DST_ALPHA           = GL_DST_ALPHA,
    ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
    ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA
};

enum class BlenidngDestination : GLenum
{
    ZERO                = GL_ZERO,
    ONE                 = GL_ONE,
    SRC_COLOR           = GL_SRC_COLOR,
    ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
    SRC_ALPHA           = GL_SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
    DST_COLOR           = GL_DST_COLOR,
    DST_ALPHA           = GL_DST_ALPHA,
    ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
    ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA
};

enum class Side
{
    FRONT = 0,
    BACK,
    BOTH
};

enum class DepthFunc : GLenum
{
    NEVER         = GL_NEVER,
    ALWAYS        = GL_ALWAYS,
    LESS          = GL_LESS,
    LESS_EQUAL    = GL_LEQUAL,
    GREATER_EQUAL = GL_GEQUAL,
    GREATER       = GL_GREATER,
    NOT_EQUAL     = GL_NOTEQUAL

};

struct BlendingSetup
{
    float alpha_test{ 0.0f };

    rend::BlenidngSource blend_src{ rend::BlenidngSource::SRC_ALPHA };
    rend::BlenidngDestination blend_dst{ rend::BlenidngDestination::ONE_MINUS_SRC_ALPHA };

    rend::BlenidngSource blend_src_alpha{ rend::BlenidngSource::SRC_ALPHA };
    rend::BlenidngDestination blend_dst_alpha{
        rend::BlenidngDestination::ONE_MINUS_SRC_ALPHA
    };

    rend::BlenidngEquation blend_equation_alpha{ 0 };
    rend::BlenidngEquation blend_equation{ rend::BlenidngEquation::ADD };

    bool blending{ false };
};

struct Colors
{

    static constexpr glm::vec3 black = glm::vec3(0.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);

    static constexpr glm::vec3 red   = glm::vec3(1.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 blue  = glm::vec3(0.0f, 1.0f, 0.0f);
    static constexpr glm::vec3 green = glm::vec3(0.0f, 0.0f, 1.0f);

    static constexpr glm::vec3 cyan    = glm::vec3(0.0f, 1.0f, 1.0f);
    static constexpr glm::vec3 yellow  = glm::vec3(1.0f, 1.0f, 0.0f);
    static constexpr glm::vec3 magenta = glm::vec3(1.0f, 0.0f, 1.0f);

    static constexpr glm::vec3 orange = glm::vec3(1, 0.5, 0);
};

enum class Combine
{
    ADD = 0,
    MULTIPLY,
    MIX
};


}  // namespace ay::rend

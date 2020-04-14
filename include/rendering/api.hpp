#pragma once









namespace ay::rend
{

enum class BlenidngEquation
{
    ADD,
    SUB,
    REVERSE_SUB,
    MIN,
    MAX
};

enum class BlenidngSource
{
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    DST_MINUS_SRC_ALPHA,
    ONE_MINUS_DST_ALPHA,
    DST_COLOR,
    ONE_MINUS_DST_COLOR
};

enum class BlenidngDestination
{
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    DST_MINUS_SRC_ALPHA,
    ONE_MINUS_DST_ALPHA,
    DST_COLOR,
    ONE_MINUS_DST_COLOR
};

enum class Side
{
    FRONT,
    BACK,
    BOTH
};


}



#pragma once


#include "utils.hpp"
#include "std_header.hpp"
#include "glm_header.hpp"

#include <cmath>

namespace ay::mth
{

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsequenced"
#endif

struct Easing
{

    struct Bezier
    {

        template<int steps>
        static void bezier_table(glm::vec2 P[4], glm::vec2 results[steps + 1])
        {
            static float C[(steps + 1) * 4], *K = 0;
            if (!K)
            {
                K = C;
                for (unsigned step = 0; step <= steps; ++step)
                {
                    float t         = (float)step / (float)steps;
                    C[step * 4 + 0] = (1 - t) * (1 - t) * (1 - t);
                    C[step * 4 + 1] = 3 * (1 - t) * (1 - t) * t;
                    C[step * 4 + 2] = 3 * (1 - t) * t * t;
                    C[step * 4 + 3] = t * t * t;
                }
            }

            for (unsigned step = 0; step <= steps; ++step)
            {
                glm::vec2 point = { K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x
                                      + K[step * 4 + 2] * P[2].x
                                      + K[step * 4 + 3] * P[3].x,
                                    K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y
                                      + K[step * 4 + 2] * P[2].y
                                      + K[step * 4 + 3] * P[3].y };
                results[step]   = point;
            }
        }

        static float cubic(float dt01, float P[4])
        {
            constexpr int STEPS = 256;
            glm::vec2 Q[4]      = { { 0, 0 }, { P[0], P[1] }, { P[2], P[3] }, { 1, 1 } };
            glm::vec2 results[STEPS + 1];
            bezier_table<STEPS>(Q, results);
            return results[(int)((dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01) * STEPS)].y;
        }
    };

    static float linear(float k)
    {
        return k;
    }

    struct Quadratic
    {
        static float in(float k)
        {
            return k * k;
        }

        static float out(float k)
        {
            return k * (2.0f - k);
        }

        static float in_out(float k)
        {
            if ((k *= 2.0f) < 1.0f)
                return 0.5f * k * k;
            return -0.5f * ((k -= 1.0f) * (k - 2.0f) - 1.0f);
        }

        static float bezier(float k, float c = 1.0)
        {
            return c * 2 * k * (1 - k) + k * k;
        }
    };

    struct Cubic
    {
        static float in(float k)
        {
            return k * k * k;
        }

        static float out(float k)
        {
            return 1.0f + ((k -= 1.0f) * k * k);
        }

        static float in_out(float k)
        {
            if ((k *= 2.0f) < 1.0f)
                return 0.5f * k * k * k;
            return 0.5f * ((k -= 2.0f) * k * k + 2.0f);
        }

        static float bezier(float k, float c)
        {
            return c * 2 * k * (1 - k) + k * k;
        }
    };

    struct Sinusoidal
    {
        static float in(float k)
        {
            return 1.0f - std::cos(k * mth::PI / 2.0f);
        }

        static float out(float k)
        {
            return std::sin(k * mth::PI / 2.0f);
        }

        static float in_out(float k)
        {
            return 0.5f * (1.0f - std::cos(mth::PI * k));
        }
    };

    struct Exponential
    {
        static float in(float k)
        {
            return k == 0.0f ? 0.0f : std::pow(1024.0f, k - 1.0f);
        }

        static float out(float k)
        {
            return k == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * k);
        }

        static float in_out(float k)
        {
            if (k == 0.0f)
                return 0.0f;
            if (k == 1.0f)
                return 1.0f;
            if ((k *= 2.0f) < 1.0f)
                return 0.5f * std::pow(1024.0f, k - 1.0f);
            return 0.5f * (-std::pow(2.0f, -10.0f * (k - 1.0f)) + 2.0f);
        }
    };

    struct Circular
    {

        static float in(float k)
        {
            return 1.0f - std::sqrt(1.0f - k * k);
        }

        static float out(float k)
        {
            return std::sqrt(1.0f - ((k -= 1.0f) * k));
        }

        static float in_out(float k)
        {
            if ((k *= 2.0f) < 1.0f)
                return -0.5f * (std::sqrt(1.0f - k * k) - 1);
            return 0.5f * (std::sqrt(1.0f - (k -= 2.0f) * k) + 1.0f);
        }
    };

    struct Elastic
    {

        static float in(float k)
        {
            if (k == 0)
                return 0;
            if (k == 1)
                return 1;
            return -std::pow(2.0f, 10.0f * (k -= 1.0f))
                   * std::sin((k - 0.1f) * (2.0f * mth::PI) / 0.4f);
        }

        static float out(float k)
        {
            if (k == 0)
                return 0;
            if (k == 1)
                return 1;
            return std::pow(2.0f, -10.0f * k)
                     * std::sin((k - 0.1f) * (2.0f * mth::PI) / 0.4f)
                   + 1.0f;
        }

        static float in_out(float k)
        {
            if ((k *= 2.0f) < 1.0f)
                return -0.5f * std::pow(2.0f, 10.0f * (k -= 1.0f))
                       * std::sin((k - 0.1f) * (2.0f * mth::PI) / 0.4f);
            return std::pow(2.0f, -10.0f * (k -= 1.0f))
                     * std::sin((k - 0.1f) * (2.0f * mth::PI) / 0.4f) * 0.5f
                   + 1.0f;
        }
    };

    struct Back
    {
        constexpr static float s  = 1.70158f;
        constexpr static float s2 = 2.5949095f;

        static float in(float k)
        {
            return k * k * ((s + 1.0f) * k - s);
        }

        static float out(float k)
        {
            return (k -= 1.0f) * k * ((s + 1.0f) * k + s) + 1.0f;
        }

        static float in_out(float k)
        {
            if ((k *= 2.0f) < 1.0f)
                return 0.5f * (k * k * ((s2 + 1.0f) * k - s2));
            return 0.5f * ((k -= 2.0f) * k * ((s2 + 1.0f) * k + s2) + 2.0f);
        }
    };

    struct Bounce
    {
        static float in(float k)
        {
            return 1.0f - out(1.0f - k);
        }

        static float out(float k)
        {
            if (k < (1.0f / 2.75f))
            {
                return 7.5625f * k * k;
            }
            else if (k < (2.0f / 2.75f))
            {
                return 7.5625f * (k -= (1.5f / 2.75f)) * k + 0.75f;
            }
            else if (k < (2.5f / 2.75f))
            {
                return 7.5625f * (k -= (2.25f / 2.75f)) * k + 0.9375f;
            }
            else
            {
                return 7.5625f * (k -= (2.625f / 2.75f)) * k + 0.984375f;
            }
        }

        static float in_out(float k)
        {
            if (k < 0.5f)
                return in(k * 2.0f) * 0.5f;
            return out(k * 2.0f - 1.0f) * 0.5f + 0.5f;
        }
    };
};

inline static std::string interpolation_names[] = {
    "Linear",
    "Quadratic_in",
    "Quadratic_out",
    "Quadratic_in_out",
    "Cubic_in",
    "Cubic_out",
    "Cubic_in_out",
    "Sinusoidal_in",
    "Sinusoidal_out",
    "Sinusoidal_in_out",
    "Exponential_in",
    "Exponential_out",
    "Exponential_in_out",
    "Circular_in",
    "Circular_out",
    "Circular_in_out",
    "Elastic_in",
    "Elastic_out",
    "Elastic_in_out",
    "Back_in",
    "Back_out",
    "Back_in_out",
    "Bounce_in",
    "Bounce_out",
    "Bounce_in_out",
};

inline static float (*interpolations[])(float) = {
    &Easing::linear,
    &Easing::Quadratic::in,
    &Easing::Quadratic::out,
    &Easing::Quadratic::in_out,
    &Easing::Cubic::in,
    &Easing::Cubic::out,
    &Easing::Cubic::in_out,
    &Easing::Sinusoidal::in,
    &Easing::Sinusoidal::out,
    &Easing::Sinusoidal::in_out,
    &Easing::Exponential::in,
    &Easing::Exponential::out,
    &Easing::Exponential::in_out,
    &Easing::Circular::in,
    &Easing::Circular::out,
    &Easing::Circular::in_out,
    &Easing::Elastic::in,
    &Easing::Elastic::out,
    &Easing::Elastic::in_out,
    &Easing::Back::in,
    &Easing::Back::out,
    &Easing::Back::in_out,
    &Easing::Bounce::in,
    &Easing::Bounce::out,
    &Easing::Bounce::in_out,
};

enum class EasingType
{
    STANDARD = 0,
    BEZIER,
};

struct EasingFun
{
    EasingType type{ EasingType::STANDARD };
    std::function<float(float)> fun{ &mth::Easing::linear };
    size_t index{ 0 };
    std::array<float, 5> params;

    void set_from_index(size_t i)
    {
        index = i;
        fun   = mth::interpolations[index];
    }

    void make_bezier()
    {
        fun = [&](float t) {
            return mth::Easing::Bezier::cubic(
              t, (float[]){ params[0], params[1], params[2], params[3] });
        };
    }

    std::array<float, 5> &parameters()
    {
        return params;
    }

    float operator()(float t)
    {
        return fun(t);
    }
};


#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

}  // namespace ay::mth

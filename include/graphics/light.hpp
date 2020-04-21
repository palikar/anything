#pragma once


#include "std_header.hpp"
#include "glm_header.hpp"


namespace ay::grph
{

static constexpr size_t MAX_LIGHT = 5;

struct DirectinalLight
{
    bool active{ false };

    glm::vec3 dir{ 0.0f, -1.0f, 0.0f };

    glm::vec3 color{ 0.0f, 0.0f, 0.0f };

    float intensity{ 1.0f };
};

struct AmbientLight
{
    bool active{ false };

    glm::vec3 color{ 0.0f, 0.0f, 0.0f };
};

struct PointLight
{
    bool active{ false };

    glm::vec3 color{ 0.0f, 0.0f, 0.0f };

    glm::vec3 position{ 0.0f, 0.0f, 0.0f };

    float linear{ 1.0f };
    float constant{ 1.0f };
    float quadratic{ 1.0f };
};

struct SpotLight
{

    bool active{ false };

    glm::vec3 color{ 0.0f, 0.0f, 0.0f };

    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 dir{ 0.0f, 0.0f, 0.0f };

    float cut_off{ 2.0 };
    float outer_cut_off{ 5.0 };
};

struct LightSetup
{

    DirectinalLight directional_light{};

    AmbientLight ambient_light{};

    std::array<PointLight, MAX_LIGHT> point_lights{};

    std::array<SpotLight, MAX_LIGHT> spot_lights{};
};


}  // namespace ay::grph

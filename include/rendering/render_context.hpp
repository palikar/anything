#pragma once

#include "graphics/light.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::rend
{

struct RenderContext
{

    glm::vec3 camera_pos;
    grph::LightSetup *light_setup;
};

}  // namespace ay::rend

#pragma once

#include "graphics/light.hpp"
#include "graphics/fog.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::rend
{

struct RenderContext
{

    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 camera_pos;
    grph::LightSetup *light_setup;
    grph::Fog fog;
    
};

}  // namespace ay::rend

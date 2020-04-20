#pragma once

#include "application/input.hpp"

#include "engine/camera.hpp"

#include "math_header.hpp"
#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::mth
{

class Raycaster
{
  private:
    size_t viewport_width;
    size_t viewport_height;

  public:
    Raycaster()
    {
    }


    void update_viewport(size_t t_width, size_t t_height)
    {
        viewport_height = t_height;
        viewport_width = t_width;
    }

    mth::Ray from_mouse(gmt::Camera& camera)
    {
        
        
        auto coord = app::Input::mouse_pos();

        const glm::vec3 norm_coord = {2.0f * coord.x / viewport_width - 1.0f,
                                      1.0f - 2.0f * coord.y / viewport_height,
                                      1.0f};
        
        const glm::vec4 clip_coord = {norm_coord.xy(), -1.0f, 1.0f};
        
        const glm::vec4 eye_coord = {(glm::inverse(camera.projection())*clip_coord).xy(), -1.0f, 0.0f};

        const glm::vec3 world_coord = (glm::inverse(camera.view()) * eye_coord).xyz();

        return mth::Ray{camera.pos(), glm::normalize(world_coord)};
    }
    

  private:
    
    
    
    


};

}

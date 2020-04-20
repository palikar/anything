#pragma once

#include "application/input.hpp"

#include "engine/camera.hpp"

#include "math_header.hpp"
#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::gmt
{

class Raycaster
{
  private:
    size_t viewport_width;
    size_t viewport_height;

    Camera *camera;

  public:
    Raycaster()
    {
    }

    void update_viewport(size_t t_width, size_t t_height)
    {
        viewport_height = t_height;
        viewport_width  = t_width;
    }

    void update_camera(Camera *t_camera)
    {
        camera = t_camera;
    }

    mth::Ray camera_to_mouse();

    mth::Ray from_position(glm::vec2 pos);

    mth::Ray from_position(glm::vec2 pos, glm::mat4 proj, glm::mat4 view);
};

}  // namespace ay::gmt

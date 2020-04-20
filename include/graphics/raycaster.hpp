#pragma once


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
        viewport_width  = t_width;
    }
};

}  // namespace ay::mth

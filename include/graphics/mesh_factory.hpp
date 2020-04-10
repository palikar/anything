#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "rendering/buffer_factory.hpp"

#include "graphics/mesh.hpp"

#include "commons.hpp"

#include <glm/glm.hpp>

namespace ay
{


inline Mesh create_cube(){

    auto buffers = make_vertex_array(
        make_index_buffer({
                {0, 1, 2},
                {2, 3, 0},
            
                {1, 5, 6},
                {6, 2, 1},
            
                {7, 6, 5},
                {5, 4, 7},
            
                {4, 0, 3},
                {3, 7, 4},
            
                {4, 5, 1},
                {1, 0, 4},
                
                {3, 2, 6},
                {6, 7, 3}
            }),
        make_vertex_buffer({
                {-1.0, -1.0,  1.0},
                    {1.0, -1.0,  1.0},
                    {1.0,  1.0,  1.0},
                    {-1.0,  1.0,  1.0},

                    {-1.0, -1.0, -1.0},
                    {1.0, -1.0, -1.0},
                    {1.0,  1.0, -1.0},
                    {-1.0,  1.0, -1.0}
                }));

    return Mesh(std::move(buffers));

}


}

#pragma once

#include "rendering/buffer_factory.hpp"
#include "graphics/mesh.hpp"

#include "commons.hpp"
#include "std_header.hpp"

#include "glm_header.hpp"

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


inline Mesh create_plane(size_t width,
                         size_t height,
                         float width_segments,
                         float height_segments)
{

    const auto half_width = width / 2.0f;
    const auto half_height = height / 2.0f;

    const auto grid_x = (std::floor( width_segments ));
    const auto grid_y = (std::floor( height_segments ));
    const auto grid_x1 = grid_x + 1;
    const auto grid_y1 = grid_y + 1;
    const float segment_width = width / grid_x;
	const float segment_height = height / grid_y;

    std::vector<Index3i> indices;
    std::vector<Vertex3f> vertecies;
    
    for (size_t iy = 0; iy < grid_y1; iy ++ ) {
		auto y = iy * segment_height - half_height;
		for (size_t ix = 0; ix < grid_x1; ix ++ ) {
            auto x = ix * segment_width - half_width;
            vertecies.push_back({ x, - y, 0 });
            
			// normals.push( 0, 0, 1 );
			// uvs.push( ix / gridX );
			// uvs.push( 1 - ( iy / gridY ) );
		}
	}

	// indices

    for (size_t iy = 0; iy < grid_y; iy ++ ) {
        for (size_t ix = 0; ix < grid_x; ix ++ ) {
            auto a = static_cast<size_t>(ix + grid_x1 * iy);
			auto b = static_cast<size_t>(ix + grid_x1 * ( iy + 1 ));
            auto c = static_cast<size_t>(( ix + 1 ) + grid_x1 * ( iy + 1 ));
            auto d = static_cast<size_t>(( ix + 1 ) + grid_x1 * iy);
            
            indices.push_back({ a, b, d });
            indices.push_back({ b, c, d });
        }
    }

    return make_vertex_array(make_index_buffer(indices),
                             make_vertex_buffer(vertecies));

}

}

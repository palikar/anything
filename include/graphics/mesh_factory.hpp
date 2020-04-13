#pragma once

#include "rendering/buffer_factory.hpp"
#include "graphics/mesh.hpp"

#include "math/utils.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay
{


inline VertexArrayPtr cube_geometry(float side = 1.0f)
{

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
                {-side, -side,  side},
                {side, -side,  side},
                {side,  side,  side},
                {-side,  side,  side},

                {-side, -side, -side},
                {side, -side, -side},
                {side,  side, -side},
                {-side,  side, -side}
            }));

    return buffers;

}


inline VertexArrayPtr plane_geometry(size_t width,
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


inline VertexArrayPtr sphere_geometry(float radius,
                                      float width_segments,
                                      float height_segments,
                                      float phi_start = 0.0f,
                                      float phi_length = 2.0f * PI,
                                      float theta_start = 0.0f,
                                      float theta_length = 2.0f * PI)
{
    
    std::vector<Index3i> indices;
    std::vector<Vertex3f> vertices;
    
	radius = std::max(radius, 1.0f);

	width_segments = std::max(3.0f, std::floor( width_segments ) );
	height_segments = std::max(2.0f, std::floor( height_segments ) );
    
    float theta_end = std::min( theta_start + theta_length, PI );
    int ix, iy;
    float index = 0;
    std::vector<std::vector<size_t>> grid;
    
    for ( iy = 0; iy <= height_segments; iy ++ )
    {
        std::vector<size_t> verticesRow;
        float v = iy / height_segments;

        // float uOffset = 0;
        // if ( iy == 0 && theta_start == 0 )
        // {
        //     uOffset = 0.5 / width_segments;
        // }
        // else if ( iy == height_segments && theta_end == PI )
        // {
        //     uOffset = - 0.5 / width_segments;
        // }

        for ( ix = 0; ix <= width_segments; ix ++ )
        {
            float u = ix / width_segments;
            const float x = - radius * std::cos( phi_start + u * phi_length ) * std::sin( theta_start + v * theta_length );
            const float y = radius * std::cos( theta_start + v * theta_length );
            const float z = radius * std::sin( phi_start + u * phi_length ) * std::sin( theta_start + v * theta_length );
            vertices.push_back({x, y, z});
            // normal.copy( vertex ).normalize();
            // normals.push( normal.x, normal.y, normal.z );
            // uvs.push( u + uOffset, 1 - v );
            verticesRow.push_back(index++);
        }
        grid.push_back( verticesRow );

    }

    for ( iy = 0; iy < height_segments; iy ++ ) {

        for ( ix = 0; ix < width_segments; ix ++ ) {

            size_t a = grid[ iy ][ ix + 1 ];
            size_t b = grid[ iy ][ ix ];
            size_t c = grid[ iy + 1 ][ ix ];
            size_t d = grid[ iy + 1 ][ ix + 1 ];

            if ( iy != 0 || theta_start > 0 ) indices.push_back({ a, b, d });
            if ( iy != height_segments - 1 || theta_end < PI ) indices.push_back({ b, c, d });

        }

    }

    
    return make_vertex_array(make_index_buffer(indices),
                             make_vertex_buffer(vertices));

}

}

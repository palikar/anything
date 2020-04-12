#pragma once

#include "rendering/buffer_factory.hpp"
#include "graphics/mesh.hpp"

#include "math/utils.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay
{


inline VertexArrayPtr cube_geometry()
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
                {-1.0, -1.0,  1.0},
                    {1.0, -1.0,  1.0},
                    {1.0,  1.0,  1.0},
                    {-1.0,  1.0,  1.0},

                    {-1.0, -1.0, -1.0},
                    {1.0, -1.0, -1.0},
                    {1.0,  1.0, -1.0},
                    {-1.0,  1.0, -1.0}
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
                                      float widthSegments,
                                      float heightSegments,
                                      float phiStart = 0.0f,
                                      float phiLength = 2.0f * PI,
                                      float thetaStart = 0.0f,
                                      float thetaLength = 2.0f * PI)
{
    
    std::vector<Index3i> indices;
    std::vector<Vertex3f> vertices;
    
	radius = std::max(radius, 1.0f);

	widthSegments = std::max(3.0f, std::floor( widthSegments ) );
	heightSegments = std::max(2.0f, std::floor( heightSegments ) );
    
    float thetaEnd = std::min( thetaStart + thetaLength, PI );
    int ix, iy;
    float index = 0;
    std::vector<std::vector<size_t>> grid;
    
    for ( iy = 0; iy <= heightSegments; iy ++ )
    {
        std::vector<size_t> verticesRow;
        float v = iy / heightSegments;
        // float uOffset = 0;

        // if ( iy == 0 && thetaStart == 0 )
        // {
        //     uOffset = 0.5 / widthSegments;
        // }
        // else if ( iy == heightSegments && thetaEnd == PI )
        // {
        //     uOffset = - 0.5 / widthSegments;
        // }

        for ( ix = 0; ix <= widthSegments; ix ++ )
        {
            float u = ix / widthSegments;
            const float x = - radius * std::cos( phiStart + u * phiLength ) * std::sin( thetaStart + v * thetaLength );
            const float y = radius * std::cos( thetaStart + v * thetaLength );
            const float z = radius * std::sin( phiStart + u * phiLength ) * std::sin( thetaStart + v * thetaLength );
            vertices.push_back({x, y, z});
            // normal.copy( vertex ).normalize();
            // normals.push( normal.x, normal.y, normal.z );
            // uvs.push( u + uOffset, 1 - v );
            verticesRow.push_back(index++);
        }
        grid.push_back( verticesRow );

    }

    for ( iy = 0; iy < heightSegments; iy ++ ) {

        for ( ix = 0; ix < widthSegments; ix ++ ) {

            size_t a = grid[ iy ][ ix + 1 ];
            size_t b = grid[ iy ][ ix ];
            size_t c = grid[ iy + 1 ][ ix ];
            size_t d = grid[ iy + 1 ][ ix + 1 ];

            if ( iy != 0 || thetaStart > 0 ) indices.push_back({ a, b, d });
            if ( iy != heightSegments - 1 || thetaEnd < PI ) indices.push_back({ b, c, d });

        }

    }

    
    return make_vertex_array(make_index_buffer(indices),
                             make_vertex_buffer(vertices));

}

}

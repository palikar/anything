

#include "graphics/mesh_factory.hpp"


namespace ay::grph
{


rend::VertexArrayPtr cube_geometry(float side)
{

    auto buffers =
      rend::make_vertex_array(rend::make_index_buffer({ { 0, 1, 2 },
                                                        { 2, 3, 0 },

                                                        { 1, 5, 6 },
                                                        { 6, 2, 1 },

                                                        { 7, 6, 5 },
                                                        { 5, 4, 7 },

                                                        { 4, 0, 3 },
                                                        { 3, 7, 4 },

                                                        { 4, 5, 1 },
                                                        { 1, 0, 4 },

                                                        { 3, 2, 6 },
                                                        { 6, 7, 3 } }),
                              rend::make_vertex_buffer({ { -side, -side, side },
                                                         { side, -side, side },
                                                         { side, side, side },
                                                         { -side, side, side },

                                                         { -side, -side, -side },
                                                         { side, -side, -side },
                                                         { side, side, -side },
                                                         { -side, side, -side } }));

    return buffers;
}


rend::VertexArrayPtr
  plane_geometry(size_t width, size_t height, float width_segments, float height_segments)
{

    const auto half_width  = width / 2.0f;
    const auto half_height = height / 2.0f;

    const auto grid_x          = (std::floor(width_segments));
    const auto grid_y          = (std::floor(height_segments));
    const auto grid_x1         = grid_x + 1;
    const auto grid_y1         = grid_y + 1;
    const float segment_width  = width / grid_x;
    const float segment_height = height / grid_y;

    std::vector<Index3i> indices;
    std::vector<Vertex3f> vertecies;

    for (size_t iy = 0; iy < grid_y1; iy++)
    {
        auto y = iy * segment_height - half_height;
        for (size_t ix = 0; ix < grid_x1; ix++)
        {
            auto x = ix * segment_width - half_width;
            vertecies.push_back({ x, -y, 0 });

            // normals.push( 0, 0, 1 );
            // uvs.push( ix / gridX );
            // uvs.push( 1 - ( iy / gridY ) );
        }
    }

    // indices

    for (size_t iy = 0; iy < grid_y; iy++)
    {
        for (size_t ix = 0; ix < grid_x; ix++)
        {
            auto a = static_cast<size_t>(ix + grid_x1 * iy);
            auto b = static_cast<size_t>(ix + grid_x1 * (iy + 1));
            auto c = static_cast<size_t>((ix + 1) + grid_x1 * (iy + 1));
            auto d = static_cast<size_t>((ix + 1) + grid_x1 * iy);

            indices.push_back({ a, b, d });
            indices.push_back({ b, c, d });
        }
    }

    return rend::make_vertex_array(rend::make_index_buffer(indices),
                                   rend::make_vertex_buffer(vertecies));
}


rend::VertexArrayPtr sphere_geometry(float radius,
                                     float width_segments,
                                     float height_segments,
                                     float phi_start,
                                     float phi_length,
                                     float theta_start,
                                     float theta_length)
{

    std::vector<Index3i> indices;
    std::vector<Vertex3f> vertices;

    radius = std::max(radius, 1.0f);

    width_segments  = std::max(3.0f, std::floor(width_segments));
    height_segments = std::max(2.0f, std::floor(height_segments));

    float theta_end = std::min(theta_start + theta_length, mth::PI);
    int ix, iy;
    float index = 0;
    std::vector<std::vector<size_t>> grid;

    for (iy = 0; iy <= height_segments; iy++)
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

        for (ix = 0; ix <= width_segments; ix++)
        {
            float u       = ix / width_segments;
            const float x = -radius * std::cos(phi_start + u * phi_length)
                            * std::sin(theta_start + v * theta_length);
            const float y = radius * std::cos(theta_start + v * theta_length);
            const float z = radius * std::sin(phi_start + u * phi_length)
                            * std::sin(theta_start + v * theta_length);
            vertices.push_back({ x, y, z });
            // normal.copy( vertex ).normalize();
            // normals.push( normal.x, normal.y, normal.z );
            // uvs.push( u + uOffset, 1 - v );
            verticesRow.push_back(index++);
        }
        grid.push_back(verticesRow);
    }

    for (iy = 0; iy < height_segments; iy++)
    {

        for (ix = 0; ix < width_segments; ix++)
        {

            size_t a = grid[iy][ix + 1];
            size_t b = grid[iy][ix];
            size_t c = grid[iy + 1][ix];
            size_t d = grid[iy + 1][ix + 1];

            if (iy != 0 || theta_start > 0)
                indices.push_back({ a, b, d });
            if (iy != height_segments - 1 || theta_end < mth::PI)
                indices.push_back({ b, c, d });
        }
    }


    return rend::make_vertex_array(rend::make_index_buffer(indices),
                                   rend::make_vertex_buffer(vertices));
}


rend::VertexArrayPtr cylinder_geometry(float radiusTop,
                                       float radiusBottom,
                                       float height,
                                       float radialSegments,
                                       float heightSegments,
                                       bool openEnded,
                                       float thetaStart,
                                       float thetaLength)
{

    std::vector<Index3i> indices;
    std::vector<Vertex3f> vertices;

    float index = 0;
    std::vector<std::vector<uint32_t>> indexArray;
    float halfHeight = height / 2;
    // float groupStart = 0;

    int x, y;
    // int groupCount = 0;

    // this will be used to calculate the normal
    // float slope = ( radiusBottom - radiusTop ) / height;

    for (y = 0; y <= heightSegments; y++)
    {

        std::vector<uint32_t> indexRow;

        float v      = y / heightSegments;
        float radius = v * (radiusBottom - radiusTop) + radiusTop;

        for (x = 0; x <= radialSegments; x++)
        {
            const float u        = x / radialSegments;
            const float theta    = u * thetaLength + thetaStart;
            const float sinTheta = std::sin(theta);
            const float cosTheta = std::cos(theta);

            const float vert_x = radius * sinTheta;
            const float vert_y = -v * height + halfHeight;
            const float vert_z = radius * cosTheta;
            vertices.push_back({ vert_x, vert_y, vert_z });

            // normal
            // normal.set( sinTheta, slope, cosTheta ).normalize();
            // normals.push( normal.x, normal.y, normal.z );
            // uvs.push( u, 1 - v );
            // save index of vertex in respective row

            indexRow.push_back(index++);
        }

        // now save vertices of the row in our index array

        indexArray.push_back(indexRow);
    }


    for (x = 0; x < radialSegments; x++)
    {
        for (y = 0; y < heightSegments; y++)
        {
            size_t a = indexArray[y][x];
            size_t b = indexArray[y + 1][x];
            size_t c = indexArray[y + 1][x + 1];
            size_t d = indexArray[y][x + 1];

            indices.push_back({ a, b, d });
            indices.push_back({ b, c, d });
        }
    }


    auto generate_cap = [&](bool top) {
        int x, centerIndexStart, centerIndexEnd;
        // int groupCount = 0;
        const float radius = (top) ? radiusTop : radiusBottom;
        const float sign   = (top) ? 1 : -1;
        // save the index of the first center vertex
        centerIndexStart = index;

        for (x = 1; x <= radialSegments; x++)
        {
            vertices.push_back({ 0, halfHeight * sign, 0 });
            // normal
            // normals.push( 0, sign, 0 );
            // uv
            // uvs.push( 0.5, 0.5 );
            index++;
        }

        centerIndexEnd = index;

        for (x = 0; x <= radialSegments; x++)
        {
            const float u        = x / radialSegments;
            const float theta    = u * thetaLength + thetaStart;
            const float cosTheta = std::cos(theta);
            const float sinTheta = std::sin(theta);

            // vertex
            const float vert_x = radius * sinTheta;
            const float vert_y = halfHeight * sign;
            const float vert_z = radius * cosTheta;
            vertices.push_back({ vert_x, vert_y, vert_z });

            // // normal
            // normals.push( 0, sign, 0 );
            // // uv
            // uv.x = ( cosTheta * 0.5 ) + 0.5;
            // uv.y = ( sinTheta * 0.5 * sign ) + 0.5;
            // uvs.push( uv.x, uv.y );
            // increase index
            index++;
        }

        for (x = 0; x < radialSegments; x++)
        {

            uint32_t c = centerIndexStart + x;
            uint32_t i = centerIndexEnd + x;

            if (top)
            {
                indices.push_back({ i, i + 1, c });
            }
            else
            {
                indices.push_back({ i + 1, i, c });
            }
        }
    };

    if (openEnded)
    {
        if (radiusTop > 0)
            generate_cap(true);
        ;
        if (radiusBottom > 0)
            generate_cap(false);
    }


    return make_vertex_array(rend::make_index_buffer(indices),
                             rend::make_vertex_buffer(vertices));
}


}  // namespace ay::grph

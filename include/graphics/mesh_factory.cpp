#include "graphics/mesh_factory.hpp"
#include "rendering/buffer_factory.hpp"

#include "math/utils.hpp"

#include "std_header.hpp"


namespace ay::grph
{


rend::VertexArrayPtr cube_geometry(float width,
                                   float height,
                                   float depth,
                                   float widthSegments,
                                   float heightSegments,
                                   float depthSegments)

{


    std::vector<Index3i> indices;
    std::vector<Vertex8fg> vertecies;
    int numberOfVertices = 0;

    auto buildPlane = [&](char u,
                          char v,
                          char w,
                          float udir,
                          float vdir,
                          float t_width,
                          float t_height,
                          float t_depth,
                          float gridX,
                          float gridY) {

        float segmentWidth = t_width / gridX;
        float segmentHeight = t_height / gridY;
        float widthHalf = t_width / 2;
        float heightHalf = t_height / 2;
        float depthHalf = t_depth / 2;
        float gridX1 = gridX + 1;
        float gridY1 = gridY + 1;
        int vertexCounter = 0;
        int ix, iy;

        for ( iy = 0; iy < gridY1; iy++ ) {
            float y = iy * segmentHeight - heightHalf;
            for ( ix = 0; ix < gridX1; ix++ ) {
                float x = ix * segmentWidth - widthHalf;

                Vertex8fg vertex;

                vertex[ u ] = x * udir;
                vertex[ v ] = y * vdir;
                vertex[ w ] = depthHalf;

                vertex[ 3 + u ] = 0;
                vertex[ 3 + v ] = 0;
                vertex[ 3 + w ] = t_depth > 0 ? 1 : - 1;

                vertex[6] = ( ix / gridX );
                vertex[7] = ( 1 - ( iy / gridY ) );

                vertecies.push_back(vertex);

                vertexCounter += 1;

            };
        }

        for ( iy = 0; iy < gridY; iy++ ) {
            for ( ix = 0; ix < gridX; ix++ ) {
                size_t a = numberOfVertices + ix + gridX1 * iy;
                size_t b = numberOfVertices + ix + gridX1 * ( iy + 1 );
                size_t c = numberOfVertices + ( ix + 1 ) + gridX1 * ( iy + 1 );
                size_t d = numberOfVertices + ( ix + 1 ) + gridX1 * iy;
                indices.push_back({ a, b, d });
                indices.push_back({ b, c, d });

            }
        }

        numberOfVertices += vertexCounter;
    };


    buildPlane( 2, 1, 0, -1, -1, depth, height, width, depthSegments, heightSegments); // px
    buildPlane( 2, 1, 0,  1, -1, depth, height, -width, depthSegments, heightSegments); // nx

    buildPlane( 0, 2, 1,  1, 1, width, depth, height, widthSegments, depthSegments); // py
    buildPlane( 0, 2, 1,   1, -1, width, depth, -height, widthSegments, depthSegments); // ny

    buildPlane( 0, 1, 2,  1, -1, width, height, depth, widthSegments, heightSegments); // pz
    buildPlane( 0, 1, 2,  -1, -1, width, height, -depth, widthSegments, heightSegments); // nz

    return rend::make_vertex_array(rend::make_index_buffer(indices),
                                   rend::make_buffer(vertecies));
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
    std::vector<Vertex8fg> vertecies;

    for (size_t iy = 0; iy < grid_y1; iy++)
    {
        auto y = iy * segment_height - half_height;
        for (size_t ix = 0; ix < grid_x1; ix++)
        {
            auto x = ix * segment_width - half_width;
            vertecies.push_back({ x, -y, 0,  0, 0, 1,  ix / grid_x,  1 - ( iy / grid_y )  });
        }
    }


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
                                   rend::make_buffer(vertecies));
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
    std::vector<Vertex8fg> vertices;

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

        float uOffset = 0;
        if ( iy == 0 && theta_start == 0 )
        {
            uOffset = 0.5 / width_segments;
        }
        else if ( iy == height_segments && theta_end == mth::PI )
        {
            uOffset = - 0.5 / width_segments;
        }

        for (ix = 0; ix <= width_segments; ix++)
        {
            float u       = ix / width_segments;
            const float x = -radius * std::cos(phi_start + u * phi_length)
                * std::sin(theta_start + v * theta_length);
            const float y = radius * std::cos(theta_start + v * theta_length);
            const float z = radius * std::sin(phi_start + u * phi_length)
                * std::sin(theta_start + v * theta_length);

            auto norm = glm::normalize(glm::vec3(x, y, z));
            vertices.push_back({ x, y, z , norm.x, norm.y, norm.z, u + uOffset, 1 - v });

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
                                   rend::make_buffer(vertices));
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
    std::vector<Vertex8fg> vertices;

    float index = 0;
    std::vector<std::vector<uint32_t>> indexArray;
    float halfHeight = height / 2;


    int x, y;

    // this will be used to calculate the normal
    float slope = ( radiusBottom - radiusTop ) / height;

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

            auto norm = glm::normalize(glm::vec3( sinTheta, slope, cosTheta ));

            vertices.push_back({ vert_x, vert_y, vert_z,
                                 norm.x, norm.y, norm.z,
                                 u, 1 - v});

            indexRow.push_back(index++);
        }

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
        int p_x, centerIndexStart, centerIndexEnd;
        // int groupCount = 0;
        const float radius = (top) ? radiusTop : radiusBottom;
        const float sign   = (top) ? 1 : -1;
        // save the index of the first center vertex
        centerIndexStart = index;

        for (p_x = 1; p_x <= radialSegments; p_x++)
        {
            vertices.push_back({ 0, halfHeight * sign, 0,
                                 0, sign, 0,
                                 0.5, 0.5});
            index++;
        }

        centerIndexEnd = index;

        for (x = 0; x <= radialSegments; x++)
        {
            const float u        = p_x / radialSegments;
            const float theta    = u * thetaLength + thetaStart;
            const float cosTheta = std::cos(theta);
            const float sinTheta = std::sin(theta);

            // vertex
            const float vert_x = radius * sinTheta;
            const float vert_y = halfHeight * sign;
            const float vert_z = radius * cosTheta;
            vertices.push_back({ vert_x, vert_y, vert_z,
                                 0, sign, 0,
                                 ( cosTheta * 0.5f ) + 0.5f, ( sinTheta * 0.5f * sign ) + 0.5f });
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
                             rend::make_buffer(vertices));
}


rend::VertexArrayPtr circle_geometry(float radius,
                                     float segments,
                                     float thetaStart,
                                     float thetaLength)
{
    std::vector<Index3i> indices;
    std::vector<Vertex8fg> vertices;


    segments = std::max(segments, 3.0f);
    vertices.push_back({ 0, 0, 0,  0, 0, 1 , 0.5, 0.5  });

    size_t i, s;
    for ( s = 0, i = 3; s <= segments; s ++, i += 3 ) {
        float segment = thetaStart + s / segments * thetaLength;

        vertices.push_back(
            {radius * std::cos( segment ), radius * std::sin( segment ), 0.0f,
             0, 0, 1,
             (vertices[ s ][0] / radius + 1 ) / 2, ( vertices[ s ][1] / radius + 1 ) / 2
            });
    }

    for ( i = 1; i <= segments; i ++ ) {

        indices.push_back({ i, i + 1, 0 });

    }



    return make_vertex_array(rend::make_index_buffer(indices),
                             rend::make_buffer(vertices));
}



rend::VertexArrayPtr ring_geometry(float innerRadius,
                                   float outerRadius,
                                   float thetaSegments,
                                   float phiSegments,
                                   float thetaStart,
                                   float thetaLength)
{

    std::vector<Index3i> indices;
    std::vector<Vertex8fg> vertices;

    phiSegments = std::max(1.0f, phiSegments );
    thetaSegments = std::max(3.0f, thetaSegments );

    float segment;
    float radius = innerRadius;
    float radiusStep = ( ( outerRadius - innerRadius ) / phiSegments );
    int j, i;

    for ( j = 0; j <= phiSegments; j ++ ) {

        for ( i = 0; i <= thetaSegments; i ++ ) {
            segment = thetaStart + i / thetaSegments * thetaLength;

            const float x = radius * std::cos( segment );
            const float y = radius * std::sin( segment );

            vertices.push_back({
                    x, y, 0.0f,
                    0, 0, 1,
                    (x/outerRadius+1) / 2, (y/outerRadius+1) / 2});

        }
        radius += radiusStep;
    }

    for ( j = 0; j < phiSegments; j ++ ) {
        float thetaSegmentLevel = j * ( thetaSegments + 1 );
        for ( i = 0; i < thetaSegments; i ++ ) {
            segment = i + thetaSegmentLevel;

            size_t a = segment;
            size_t b = segment + thetaSegments + 1;
            size_t c = segment + thetaSegments + 2;
            size_t d = segment + 1;

            indices.push_back({ a, b, d });
            indices.push_back({ b, c, d });

        }

    }

    return make_vertex_array(rend::make_index_buffer(indices),
                             rend::make_buffer(vertices));
}


rend::VertexArrayPtr torus_geometry(float radius,
                                   float tube ,
                                   float radialSegments,
                                   float tubularSegments,
                                   float arc)
{


    std::vector<Index3i> indices;
    std::vector<Vertex8fg> vertices;


    radialSegments = std::floor( radialSegments );
	tubularSegments = std::floor( tubularSegments );

    int j, i;

    for ( j = 0; j <= radialSegments; j ++ ) {
        for ( i = 0; i <= tubularSegments; i ++ ) {
            float u = i / tubularSegments * arc;
            float v = j / radialSegments * mth::PI * 2;

			const float x = ( radius + tube * std::cos( v ) ) * std::cos( u );
			const float y = ( radius + tube * std::cos( v ) ) * std::sin( u );
			const float z = tube * std::sin( v );

            auto n = glm::normalize(glm::vec3(x, y, z) - glm::vec3(radius * std::cos( u ), radius * std::sin( u ), 0.0f));
            
			vertices.push_back({
                    x, y, z,
                    n.x, n.y, n.z,
                    i / tubularSegments,  j / radialSegments});
		}

	}

    for ( j = 1; j <= radialSegments; j ++ ) {
        for ( i = 1; i <= tubularSegments; i ++ ) {
            
			size_t a = ( tubularSegments + 1 ) * j + i - 1;
			size_t b = ( tubularSegments + 1 ) * ( j - 1 ) + i - 1;
			size_t c = ( tubularSegments + 1 ) * ( j - 1 ) + i;
			size_t d = ( tubularSegments + 1 ) * j + i;
            
			indices.push_back({ a, b, d });
			indices.push_back({ b, c, d });

		}

	}


    return make_vertex_array(rend::make_index_buffer(indices),
                             rend::make_buffer(vertices));

}


}  // namespace ay::grph

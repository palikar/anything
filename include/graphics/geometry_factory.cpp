#include "graphics/geometry_factory.hpp"

namespace ay::grph
{


Geometry cube_geometry(float width,
                       float height,
                       float depth,
                       float widthSegments,
                       float heightSegments,
                       float depthSegments)
{

    std::vector<float> pos;
    std::vector<float> uv;
    std::vector<float> normals;
    std::vector<uint32_t> indices;

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
        float segmentWidth  = t_width / gridX;
        float segmentHeight = t_height / gridY;
        float widthHalf     = t_width / 2;
        float heightHalf    = t_height / 2;
        float depthHalf     = t_depth / 2;
        float gridX1        = gridX + 1;
        float gridY1        = gridY + 1;
        int vertexCounter   = 0;
        int ix, iy;

        for (iy = 0; iy < gridY1; iy++)
        {
            float y = iy * segmentHeight - heightHalf;
            for (ix = 0; ix < gridX1; ix++)
            {
                float x = ix * segmentWidth - widthHalf;

                Vertex8fg vertex;

                vertex[u] = x * udir;
                vertex[v] = y * vdir;
                vertex[w] = depthHalf;

                vertex[3 + u] = 0;
                vertex[3 + v] = 0;
                vertex[3 + w] = t_depth > 0 ? 1 : -1;

                vertex[6] = (ix / gridX);
                vertex[7] = (1 - (iy / gridY));

                pos.insert(pos.end(), { vertex[0], vertex[1], vertex[2] });
                normals.insert(normals.end(), { vertex[3], vertex[4], vertex[5] });
                uv.insert(uv.end(), { ix / gridX, 1 - (iy / gridY) });

                // vertecies.push_back(vertex);

                vertexCounter += 1;
            };
        }

        for (iy = 0; iy < gridY; iy++)
        {
            for (ix = 0; ix < gridX; ix++)
            {
                uint32_t a = numberOfVertices + ix + gridX1 * iy;
                uint32_t b = numberOfVertices + ix + gridX1 * (iy + 1);
                uint32_t c = numberOfVertices + (ix + 1) + gridX1 * (iy + 1);
                uint32_t d = numberOfVertices + (ix + 1) + gridX1 * iy;

                indices.insert(indices.end(), { d, b, a, d, c, b });
                // indices.insert(indices.end(), { a, b, d, d, c, b });
            }
        }

        numberOfVertices += vertexCounter;
    };


    buildPlane(
      2, 1, 0, -1, -1, depth, height, width, depthSegments, heightSegments);  // px
    buildPlane(
      2, 1, 0, 1, -1, depth, height, -width, depthSegments, heightSegments);  // nx

    buildPlane(0, 2, 1, 1, 1, width, depth, height, widthSegments, depthSegments);  // py
    buildPlane(
      0, 2, 1, 1, -1, width, depth, -height, widthSegments, depthSegments);  // ny

    buildPlane(
      0, 1, 2, 1, -1, width, height, depth, widthSegments, heightSegments);  // pz
    buildPlane(
      0, 1, 2, -1, -1, width, height, -depth, widthSegments, heightSegments);  // nz

    Geometry geometry;
    geometry.set_attribute("position", pos, 3);
    geometry.set_attribute("normal", normals, 3);
    geometry.set_attribute("uv", uv, 2);
    geometry.set_index(indices);
    geometry.calculate_tangents();
    return geometry;
}


Geometry
  plane_geometry(size_t width, size_t height, float width_segments, float height_segments)
{


    std::vector<float> pos;
    std::vector<float> uv;
    std::vector<float> normals;
    std::vector<uint32_t> indices;

    const auto half_width  = width / 2.0f;
    const auto half_height = height / 2.0f;

    const auto grid_x          = (std::floor(width_segments));
    const auto grid_y          = (std::floor(height_segments));
    const auto grid_x1         = grid_x + 1;
    const auto grid_y1         = grid_y + 1;
    const float segment_width  = width / grid_x;
    const float segment_height = height / grid_y;

    for (size_t iy = 0; iy < grid_y1; iy++)
    {
        auto y = iy * segment_height - half_height;
        for (size_t ix = 0; ix < grid_x1; ix++)
        {
            auto x = ix * segment_width - half_width;

            pos.insert(pos.end(), { x, -y, 0 });
            normals.insert(normals.end(), { 0, 0, -1 });
            uv.insert(uv.end(), { ix / grid_x, 1 - (iy / grid_y) });
        }
    }


    for (size_t iy = 0; iy < grid_y; iy++)
    {
        for (size_t ix = 0; ix < grid_x; ix++)
        {
            uint32_t a = static_cast<size_t>(ix + grid_x1 * iy);
            uint32_t b = static_cast<size_t>(ix + grid_x1 * (iy + 1));
            uint32_t c = static_cast<size_t>((ix + 1) + grid_x1 * (iy + 1));
            uint32_t d = static_cast<size_t>((ix + 1) + grid_x1 * iy);

            indices.insert(indices.end(), { a, b, d, b, c, d });
        }
    }


    Geometry geometry;
    geometry.set_attribute("position", pos, 3);
    geometry.set_attribute("normal", normals, 3);
    geometry.set_attribute("uv", uv, 2);
    geometry.set_index(indices);
    geometry.calculate_tangents();
    return geometry;
}


Geometry sphere_geometry(float radius,
                         float width_segments,
                         float height_segments,
                         float phi_start,
                         float phi_length,
                         float theta_start,
                         float theta_length)
{


    std::vector<float> pos;
    std::vector<float> uv;
    std::vector<float> normals;
    std::vector<uint32_t> indices;

    radius = std::max(radius, 1.0f);

    width_segments  = std::max(3.0f, std::floor(width_segments));
    height_segments = std::max(2.0f, std::floor(height_segments));

    float theta_end = std::min(theta_start + theta_length, mth::PI);
    int ix, iy;
    float index = 0;
    std::vector<std::vector<size_t>> grid;

    for (iy = 0; iy <= height_segments; ++iy)
    {
        std::vector<size_t> verticesRow;
        float v = iy / height_segments;

        float uOffset = 0;
        if (iy == 0 && theta_start == 0)
        {
            uOffset = 0.5f / width_segments;
        }
        else if (iy == height_segments && theta_end == mth::PI)
        {
            uOffset = -0.5f / width_segments;
        }

        for (ix = 0; ix <= width_segments; ix++)
        {
            float u       = ix / width_segments;
            const float x = -radius * std::cos(phi_start + u * phi_length)
                            * std::sin(theta_start + v * theta_length);
            const float y = radius * std::cos(theta_start + v * theta_length);
            const float z = radius * std::sin(phi_start + u * phi_length)
                            * std::sin(theta_start + v * theta_length);

            pos.insert(pos.end(), { x, y, z });

            auto norm = glm::normalize(glm::vec3(x, y, z));
            normals.insert(normals.end(), { norm.x, norm.y, norm.z });

            uv.insert(uv.end(), { u + uOffset, 1 - v });

            // vertices.push_back({ x, y, z , norm.x, norm.y, norm.z, u + uOffset, 1 - v
            // });

            verticesRow.push_back(index++);
        }

        grid.push_back(verticesRow);
    }

    for (iy = 0; iy < height_segments; ++iy)
    {

        for (ix = 0; ix < width_segments; ++ix)
        {

            uint32_t a = grid[iy][ix + 1];
            uint32_t b = grid[iy][ix];
            uint32_t c = grid[iy + 1][ix];
            uint32_t d = grid[iy + 1][ix + 1];

            if (iy != 0 || theta_start > 0)
            {
                // indices.insert(indices.end(), { a, b, d });
                indices.insert(indices.end(), { d, b, a });
            }

            if (iy != height_segments - 1 || theta_end < mth::PI)
            {
                // indices.insert(indices.end(), { b, c, d });
                indices.insert(indices.end(), { d, c, b });
            }
        }
    }

    Geometry geometry;
    geometry.set_attribute("position", std::move(pos), 3);
    geometry.set_attribute("normal", std::move(normals), 3);
    geometry.set_attribute("uv", std::move(uv), 2);
    geometry.set_index(indices);
    geometry.calculate_tangents();
    return geometry;
}

Geometry cylinder_geometry(float radiusTop,
                           float radiusBottom,
                           float height,
                           float radialSegments,
                           float heightSegments,
                           bool openEnded,
                           float thetaStart,
                           float thetaLength)
{


    std::vector<float> pos;
    std::vector<float> uv;
    std::vector<float> normals;
    std::vector<uint32_t> indices;

    float index = 0;
    std::vector<std::vector<uint32_t>> indexArray;
    float halfHeight = height / 2;

    int x, y;

    // this will be used to calculate the normal
    float slope = (radiusBottom - radiusTop) / height;

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

            auto norm = glm::normalize(glm::vec3(sinTheta, slope, cosTheta));

            pos.insert(pos.end(), { vert_x, vert_y, vert_z });
            normals.insert(normals.end(), { norm.x, norm.y, norm.z });
            uv.insert(uv.end(), { u, 1 - v });

            indexRow.push_back(index++);
        }

        indexArray.push_back(indexRow);
    }

    for (x = 0; x < radialSegments; x++)
    {
        for (y = 0; y < heightSegments; y++)
        {
            uint32_t a = indexArray[y][x];
            uint32_t b = indexArray[y + 1][x];
            uint32_t c = indexArray[y + 1][x + 1];
            uint32_t d = indexArray[y][x + 1];

            indices.insert(indices.end(), { b, a, d, c, b, d });
        }
    }

    const auto generate_cap = [&](bool top) {
        int p_x, centerIndexStart, centerIndexEnd;

        const float radius = (top) ? radiusTop : radiusBottom;
        const float sign   = (top) ? 1.0 : -1.0;

        centerIndexStart = index;

        for (p_x = 1; p_x <= radialSegments; p_x++)
        {
            pos.insert(pos.end(), { 0, halfHeight * sign, 0 });
            normals.insert(normals.end(), { 0, sign, 0 });
            uv.insert(uv.end(), { 0.5, 0.5 });

            // vertices.push_back({ 0, halfHeight * sign, 0,
            //                      0, sign, 0,
            //                      0.5, 0.5});

            ++index;
        }

        centerIndexEnd = index;

        for (p_x = 0; p_x <= radialSegments; p_x++)
        {
            const float u        = p_x / radialSegments;
            const float theta    = u * thetaLength + thetaStart;
            const float cosTheta = std::cos(theta);
            const float sinTheta = std::sin(theta);

            // vertex
            const float vert_x = radius * sinTheta;
            const float vert_y = halfHeight * sign;
            const float vert_z = radius * cosTheta;

            pos.insert(pos.end(), { vert_x, vert_y, vert_z });
            normals.insert(normals.end(), { 0, sign, 0 });
            uv.insert(uv.end(),
                      { (cosTheta * 0.5f) + 0.5f, (sinTheta * 0.5f * sign) + 0.5f });

            index++;
        }

        for (p_x = 0; p_x < radialSegments; ++p_x)
        {

            uint32_t c = centerIndexStart + p_x;
            uint32_t i = centerIndexEnd + p_x;

            if (top)
            {
                indices.insert(indices.end(), { c, i + 1, i });
            }
            else
            {
                indices.insert(indices.end(), { i + 1, c, i });
            }
        }
    };

    if (!openEnded)
    {
        if (radiusTop > 0)
        {
            generate_cap(true);
        }

        if (radiusBottom > 0)
        {
            generate_cap(false);
        }
    }


    Geometry geometry;
    geometry.set_attribute("position", pos, 3);
    geometry.set_attribute("normal", normals, 3);
    geometry.set_attribute("uv", uv, 2);
    geometry.set_index(indices);
    geometry.calculate_tangents();
    return geometry;
}


Geometry
  circle_geometry(float radius, float segments, float thetaStart, float thetaLength)
{


    std::vector<float> pos;
    std::vector<float> uv;
    std::vector<float> normals;
    std::vector<uint32_t> indices;


    segments = std::max(segments, 3.0f);
    pos.insert(pos.end(), { 0, 0, 0 });
    normals.insert(normals.end(), { 0, 0, 1 });
    uv.insert(uv.end(), { 0.5, 0.5 });
    // vertices.push_back({,   ,   });

    uint32_t i, s;
    for (s = 0, i = 3; s <= segments; s++, i += 3)
    {
        float segment = thetaStart + s / segments * thetaLength;

        pos.insert(pos.end(),
                   { radius * std::cos(segment), radius * std::sin(segment), 0.0f });
        normals.insert(normals.end(), { 0, 0, 1 });
        uv.insert(uv.end(), { (pos[i] / radius + 1) / 2, (pos[i + 1] / radius + 1) / 2 });

        // vertices.push_back(
        //     {radius * std::cos( segment ), radius * std::sin( segment ), 0.0f,
        //      0, 0, 1,
        //      (vertices[ s ][0] / radius + 1 ) / 2, ( vertices[ s ][1] / radius + 1 ) /
        //      2
        //     });
    }

    for (i = 1; i <= segments; i++)
    {
        indices.insert(indices.end(), { i, i + 1, 0 });
        // indices.push_back({ i, i + 1, 0 });
    }

    Geometry geometry;
    geometry.set_attribute("position", pos, 3);
    geometry.set_attribute("normal", normals, 3);
    geometry.set_attribute("uv", uv, 2);
    geometry.set_index(indices);
    geometry.calculate_tangents();
    return geometry;
}


Geometry ring_geometry(float innerRadius,
                       float outerRadius,
                       float thetaSegments,
                       float phiSegments,
                       float thetaStart,
                       float thetaLength)
{


    std::vector<float> pos;
    std::vector<float> uv;
    std::vector<float> normals;
    std::vector<uint32_t> indices;


    phiSegments   = std::max(1.0f, phiSegments);
    thetaSegments = std::max(3.0f, thetaSegments);

    float segment;
    float radius     = innerRadius;
    float radiusStep = ((outerRadius - innerRadius) / phiSegments);
    int j, i;

    for (j = 0; j <= phiSegments; j++)
    {

        for (i = 0; i <= thetaSegments; i++)
        {
            segment = thetaStart + i / thetaSegments * thetaLength;

            const float x = radius * std::cos(segment);
            const float y = radius * std::sin(segment);

            pos.insert(pos.end(), { x, y, 0.0f });
            normals.insert(normals.end(), { 0, 0, 1 });
            uv.insert(uv.end(), { (x / outerRadius + 1) / 2, (y / outerRadius + 1) / 2 });

            // vertices.push_back({
            //         x, y, 0.0f,
            //         0, 0, 1,
            //         (x/outerRadius+1) / 2, (y/outerRadius+1) / 2});
        }
        radius += radiusStep;
    }

    for (j = 0; j < phiSegments; j++)
    {
        float thetaSegmentLevel = j * (thetaSegments + 1);
        for (i = 0; i < thetaSegments; i++)
        {
            segment = i + thetaSegmentLevel;

            uint32_t a = segment;
            uint32_t b = segment + thetaSegments + 1;
            uint32_t c = segment + thetaSegments + 2;
            uint32_t d = segment + 1;

            indices.insert(indices.end(), { a, b, d, b, c, d });
            // indices.push_back({ a, b, d });
            // indices.push_back({ b, c, d });
        }
    }

    Geometry geometry;
    geometry.set_attribute("position", pos, 3);
    geometry.set_attribute("normal", normals, 3);
    geometry.set_attribute("uv", uv, 2);
    geometry.set_index(indices);
    geometry.calculate_tangents();
    return geometry;
}

Geometry torus_geometry(float radius,
                        float tube,
                        float radialSegments,
                        float tubularSegments,
                        float arc)
{

    std::vector<float> pos;
    std::vector<float> uv;
    std::vector<float> normals;
    std::vector<uint32_t> indices;


    radialSegments  = std::floor(radialSegments);
    tubularSegments = std::floor(tubularSegments);

    int j, i;

    for (j = 0; j <= radialSegments; j++)
    {
        for (i = 0; i <= tubularSegments; i++)
        {
            float u = i / tubularSegments * arc;
            float v = j / radialSegments * mth::PI * 2;

            const float x = (radius + tube * std::cos(v)) * std::cos(u);
            const float y = (radius + tube * std::cos(v)) * std::sin(u);
            const float z = tube * std::sin(v);

            auto n = glm::normalize(
              glm::vec3(x, y, z)
              - glm::vec3(radius * std::cos(u), radius * std::sin(u), 0.0f));

            pos.insert(pos.end(), { x, y, z });
            normals.insert(normals.end(), { n.x, n.y, n.z });
            uv.insert(uv.end(), { i / tubularSegments, j / radialSegments });
        }
    }

    for (j = 1; j <= radialSegments; j++)
    {
        for (i = 1; i <= tubularSegments; i++)
        {

            uint32_t a = (tubularSegments + 1) * j + i - 1;
            uint32_t b = (tubularSegments + 1) * (j - 1) + i - 1;
            uint32_t c = (tubularSegments + 1) * (j - 1) + i;
            uint32_t d = (tubularSegments + 1) * j + i;

            indices.insert(indices.end(), { d, b, a, d, c, b });
        }
    }

    Geometry geometry;
    geometry.set_attribute("position", pos, 3);
    geometry.set_attribute("normal", normals, 3);
    geometry.set_attribute("uv", uv, 2);
    geometry.set_index(indices);
    geometry.calculate_tangents();
    return geometry;
}


Geometry cone_geometry(float radius,
                       float height,
                       float radialSegments,
                       float heightSegments,
                       bool openEnded,
                       float thetaStart,
                       float thetaLength)
{

    return cylinder_geometry(0,
                             radius,
                             height,
                             radialSegments,
                             heightSegments,
                             openEnded,
                             thetaStart,
                             thetaLength);
}

}  // namespace ay::grph

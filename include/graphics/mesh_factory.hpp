#pragma once

#include "rendering/vertex_array.hpp"
#include "math/utils.hpp"


namespace ay::grph
{



rend::VertexArrayPtr cube_geometry(float width = 1.0,
                                   float height= 1.0,
                                   float depth = 1.0,
                                   float widthSegments = 1.0,
                                   float heightSegments= 1.0,
                                   float depthSegments = 1.0);


rend::VertexArrayPtr plane_geometry(size_t width,
                                    size_t height,
                                    float width_segments,
                                    float height_segments);


rend::VertexArrayPtr sphere_geometry(float radius,
                                     float width_segments,
                                     float height_segments,
                                     float phi_start    = 0.0f,
                                     float phi_length   = 2.0f * mth::PI,
                                     float theta_start  = 0.0f,
                                     float theta_length = 2.0f * mth::PI);

rend::VertexArrayPtr cylinder_geometry(float radiusTop,
                                       float radiusBottom,
                                       float height,
                                       float radialSegments = 8.0f,
                                       float heightSegments = 1.0f,
                                       bool openEnded       = false,
                                       float thetaStart     = 0.0f,
                                       float thetaLength    = 2.0f * mth::PI);


}  // namespace ay::grph

#pragma once

#include "rendering/buffer_factory.hpp"
#include "graphics/mesh.hpp"

#include "math/utils.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::grph
{


rend::VertexArrayPtr cube_geometry(float side = 1.0f);


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

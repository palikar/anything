#pragma once

#include "rendering/vertex_array.hpp"

#include "graphics/geometry.hpp"

#include "math/utils.hpp"


namespace ay::grph
{


Geometry cube_geometry(float width          = 1.0,
                       float height         = 1.0,
                       float depth          = 1.0,
                       float widthSegments  = 1.0,
                       float heightSegments = 1.0,
                       float depthSegments  = 1.0);


Geometry plane_geometry(size_t width,
                        size_t height,
                        float width_segments,
                        float height_segments);


Geometry sphere_geometry(float radius,
                         float width_segments,
                         float height_segments,
                         float phi_start    = 0.0f,
                         float phi_length   = 2.0f * mth::PI,
                         float theta_start  = 0.0f,
                         float theta_length = 2.0f * mth::PI);

Geometry cylinder_geometry(float radiusTop,
                           float radiusBottom,
                           float height,
                           float radialSegments = 10.0f,
                           float heightSegments = 1.0f,
                           bool openEnded       = false,
                           float thetaStart     = 0.0f,
                           float thetaLength    = 2.0f * mth::PI);


Geometry circle_geometry(float radius      = 1,
                         float segments    = 8,
                         float thetaStart  = 0.0f,
                         float thetaLength = 2.0f * mth::PI);


Geometry ring_geometry(float innerRadius   = 0.5,
                       float outerRadius   = 1,
                       float thetaSegments = 8,
                       float phiSegments   = 1,
                       float thetaStart    = 0.0f,
                       float thetaLength   = 2.0f * mth::PI);

Geometry torus_geometry(float radius          = 1.0,
                        float tube            = 0.4,
                        float radialSegments  = 8,
                        float tubularSegments = 6,
                        float arc             = 2.0f * mth::PI);


Geometry cone_geometry(float radius         = 1.0,
                       float height         = 2.0,
                       float radialSegments = 10.0f,
                       float heightSegments = 1.0f,
                       bool openEnded       = false,
                       float thetaStart     = 0.0f,
                       float thetaLength    = 2.0f * mth::PI);

}  // namespace ay::grph

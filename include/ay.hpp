#pragma once

#include "glm_header.hpp"
#include "std_header.hpp"
#include "macros.hpp"
#include "commons.hpp"

#include "rendering/buffer_factory.hpp"
#include "rendering/renderer.hpp"
#include "rendering/renderer3D.hpp"
#include "rendering/shaders.hpp"
#include "rendering/texture.hpp"
#include "rendering/cube_texture.hpp"
#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"
#include "rendering/frame_buffer.hpp"

#include "engine/camera.hpp"
#include "engine/engine.hpp"
#include "engine/component.hpp"
#include "engine/entity.hpp"
#include "engine/entity_factory.hpp"
#include "engine/game_base.hpp"
#include "engine/scene.hpp"

#include "engine/entities/camera_helper.hpp"
#include "engine/entities/axis_helper.hpp"
#include "engine/entities/plane_helper.hpp"
#include "engine/entities/skybox.hpp"
#include "engine/entities/grid_helper.hpp"

#include "engine/components/orbital_camera.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/line_segments.hpp"
#include "engine/components/transform.hpp"
#include "engine/components/group.hpp"
#include "engine/components/clock.hpp"

#include "math/box.hpp"
#include "math/line.hpp"
#include "math/plane.hpp"
#include "math/ray.hpp"
#include "math/sphere.hpp"
#include "math/transform.hpp"
#include "math/utils.hpp"

#include "application/application.hpp"
#include "application/event.hpp"
#include "application/input.hpp"
#include "application/resource_loader.hpp"
#include "application/keycodes.hpp"
#include "application/window.hpp"

#include "graphics/geometry.hpp"
#include "graphics/geometry_factory.hpp"
#include "graphics/mesh.hpp"
#include "graphics/line_segments.hpp"
#include "graphics/material.hpp"
#include "graphics/material_builder.hpp"
#include "graphics/mesh_factory.hpp"

#include "graphics/materials/solid_color.hpp"
#include "graphics/materials/textured.hpp"

#include "graphics/loading/mesh_loader.hpp"


#include "util/meta.hpp"
#include "util/gl_helpers.hpp"

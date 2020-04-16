#pragma once

#include "graphics/geometry.hpp"
#include "graphics/geometry_factory.hpp"

#include "graphics/materials/solid_color.hpp"

#include "engine/entity.hpp"
#include "engine/entity_factory.hpp"


#include "glm_header.hpp"

namespace ay::grph
{


auto axis()
{

    constexpr float cyl_height       = 1.5;
    constexpr float cone_height      = 0.33;
    constexpr float translate_offset = -0.2;

    Geometry final_geom = cylinder_geometry(0.05, 0.05, cyl_height);
    ;
    auto cone_1 = cone_geometry(0.1, cone_height);

    cone_1.translate(0.0, (cyl_height + cone_height) / 2, 0.f);
    final_geom.merge(cone_1);

    auto cyl_2  = cylinder_geometry(0.05, 0.05, cyl_height);
    auto cone_2 = cone_geometry(0.1, cone_height);
    cone_2.translate(0.0, (cyl_height + cone_height) / 2, 0.f);

    cyl_2.merge(cone_2);
    cyl_2.rotate_x(glm::radians(90.0f));
    cyl_2.translate(0.0,
                    -(cyl_height + cone_height) / 2 - translate_offset,
                    (cyl_height + cone_height) / 2.0 + translate_offset);

    // final_geom.merge(cyl_2);

    auto cyl_3  = cylinder_geometry(0.05, 0.05, cyl_height);
    auto cone_3 = cone_geometry(0.1, cone_height);
    cone_3.translate(0.0, (cyl_height + cone_height) / 2, 0.f);

    cyl_3.merge(cone_3);
    cyl_3.rotate_z(glm::radians(-90.0f));
    cyl_3.translate((cyl_height + cone_height) / 2.0 + translate_offset,
                    -(cyl_height + cone_height) / 2 - translate_offset,
                    0.0);

    // final_geom.merge(cyl_3);


    return gmt::group_entity(
      gmt::mesh_entity(
        { std::move(final_geom), solid_color(glm::vec3(0.0f, 1.0f, 0.0f)) }),
      gmt::mesh_entity({ std::move(cyl_2), solid_color(glm::vec3(0.0f, 0.0f, 1.0f)) }),
      gmt::mesh_entity({ std::move(cyl_3), solid_color(glm::vec3(1.0f, 0.0f, 0.0f)) })

    );


    // return final_geom;
}


}  // namespace ay::grph

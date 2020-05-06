#include "engine/entities/axis_helper.hpp"


namespace ay::gmt
{

AxisHelper::AxisHelper()
{
    m_transform = add_component(std::make_unique<cmp::TransformComponent>());
    m_children  = add_component(std::make_unique<cmp::GroupComponent>());

    init_meshes();
}

void AxisHelper::init_meshes()
{

    auto arrow_1 = grph::cylinder_geometry(0.05, 0.05, cyl_height);
    auto cone_1  = grph::cone_geometry(0.1, cone_height);
    cone_1.translate(0.0, (cyl_height + cone_height) / 2, 0.f);
    arrow_1.merge(cone_1);
    arrow_1.translate(0.0, (cyl_height) / 2, 0.f);


    auto arrow_2 = grph::cylinder_geometry(0.05, 0.05, cyl_height);
    auto cone_2  = grph::cone_geometry(0.1, cone_height);
    cone_2.translate(0.0, (cyl_height + cone_height) / 2, 0.f);

    arrow_2.merge(cone_2);
    arrow_2.rotate_x(glm::radians(90.0f));
    arrow_2.translate(0.0,
                      -(cyl_height + cone_height) / 2 - translate_offset,
                      (cyl_height + cone_height) / 2.0 + translate_offset);
    arrow_2.translate(0.0, (cyl_height) / 2, 0.f);

    auto arrow_3 = grph::cylinder_geometry(0.05, 0.05, cyl_height);
    auto cone_3  = grph::cone_geometry(0.1, cone_height);
    cone_3.translate(0.0, (cyl_height + cone_height) / 2, 0.f);

    arrow_3.merge(cone_3);
    arrow_3.rotate_z(glm::radians(-90.0f));
    arrow_3.translate((cyl_height + cone_height) / 2.0 + translate_offset,
                      -(cyl_height + cone_height) / 2 - translate_offset,
                      0.0);
    arrow_3.translate(0.0, (cyl_height) / 2, 0.f);

    auto mat1 = grph::solid_color(glm::vec3(0.0f, 0.0f, 1.0f));
    mat1->no_lighting();

    auto mat2 = grph::solid_color(glm::vec3(0.0f, 1.0f, 0.0f));
    mat2->no_lighting();

    auto mat3 = grph::solid_color(glm::vec3(1.0f, 0.0f, 0.0f));
    mat3->no_lighting();

    cmp::add_children(this,
                      mesh_entity({ std::move(arrow_1), std::move(mat1) }),
                      mesh_entity({ std::move(arrow_2), std::move(mat2) }),
                      mesh_entity({ std::move(arrow_3), std::move(mat3) }));
}


}  // namespace ay::gmt

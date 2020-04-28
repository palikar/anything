#include "engine/entities/pointlight_helper.hpp"

#include "graphics/geometry_factory.hpp"

namespace ay::gmt
{

PointlightHelper::PointlightHelper(float size, glm::vec3 t_color)
  : m_size(size), m_color(t_color)
{
    m_transform = add_component(std::make_unique<cmp::TransformComponent>());

    init_points();
}

void PointlightHelper::init_points()
{
    auto sphere = grph::sphere_geometry(m_size, 4, 2);
    sphere.pack();

    m_mesh = add_component<cmp::MeshComponent>(std::move(sphere),
                                               grph::solid_color(m_color, true));
    m_mesh->mesh.material<grph::SolidColorMaterial>()->no_lighting();
}


}  // namespace ay::gmt

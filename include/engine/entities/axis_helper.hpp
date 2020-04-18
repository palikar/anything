#pragma once

#include "application/event.hpp"

#include "engine/game_base.hpp"
#include "engine/component.hpp"
#include "engine/entity.hpp"

#include "graphics/materials/solid_color.hpp"
#include "graphics/geometry_factory.hpp"


#include "engine/components/line_segments.hpp"
#include "engine/components/transform.hpp"
#include "engine/components/group.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::gmt
{


class AxisHelper : public Entity
{
  private:

    cmp::TransformComponent *m_transform;
    cmp::GroupComponent* m_children;


    static constexpr float cyl_height       = 1.5;
    static constexpr float cone_height      = 0.33;
    static constexpr float translate_offset = -0.2;

  public:
    AxisHelper() 
    {
        m_transform     = add_component(std::make_unique<cmp::TransformComponent>());
        m_children = add_component(std::make_unique<cmp::GroupComponent>());

        init_meshes();
    }

    void init_meshes()
    {

        auto arrow_1 = grph::cylinder_geometry(0.05, 0.05, cyl_height);
        auto cone_1 = grph::cone_geometry(0.1, cone_height);
        cone_1.translate(0.0, (cyl_height + cone_height) / 2, 0.f);
        arrow_1.merge(cone_1);


        auto arrow_2  = grph::cylinder_geometry(0.05, 0.05, cyl_height);
        auto cone_2 = grph::cone_geometry(0.1, cone_height);
        cone_2.translate(0.0, (cyl_height + cone_height) / 2, 0.f);

        arrow_2.merge(cone_2);
        arrow_2.rotate_x(glm::radians(90.0f));
        arrow_2.translate(0.0,
                        -(cyl_height + cone_height) / 2 - translate_offset,
                        (cyl_height + cone_height) / 2.0 + translate_offset);

        auto arrow_3  = grph::cylinder_geometry(0.05, 0.05, cyl_height);
        auto cone_3 = grph::cone_geometry(0.1, cone_height);
        cone_3.translate(0.0, (cyl_height + cone_height) / 2, 0.f);

        arrow_3.merge(cone_3);
        arrow_3.rotate_z(glm::radians(-90.0f));
        arrow_3.translate((cyl_height + cone_height) / 2.0 + translate_offset,
                        -(cyl_height + cone_height) / 2 - translate_offset,
                        0.0);


        cmp::add_children(this,
                          mesh_entity({ std::move(arrow_1), grph::solid_color(glm::vec3(0.0f, 1.0f, 0.0f)) }),
                          mesh_entity({ std::move(arrow_2), grph::solid_color(glm::vec3(0.0f, 0.0f, 1.0f)) }),
                          mesh_entity({ std::move(arrow_3), grph::solid_color(glm::vec3(1.0f, 0.0f, 0.0f)) })
            );

        
    }

    
    void update(double) override
    {}

    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override{};};


using AxisHelperPtr = std::unique_ptr<AxisHelper>;


AxisHelperPtr axis()
{
    return std::make_unique<AxisHelper>();
}


}  // namespace ay::gmt

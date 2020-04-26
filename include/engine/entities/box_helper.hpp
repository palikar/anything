#pragma once

#include "graphics/line_segments.hpp"
#include "graphics/material.hpp"
#include "graphics/material_builder.hpp"
#include "graphics/materials/solid_color.hpp"

#include "engine/entity.hpp"
#include "engine/game_base.hpp"
#include "engine/entity_factory.hpp"
#include "engine/component.hpp"
#include "engine/components/line_segments.hpp"
#include "engine/components/group.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/transform.hpp"

#include "math/box.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::gmt
{


class BoxHelper : public Entity
{
  private:
    cmp::LineSegmentsComponent *m_line_segments;
    cmp::TransformComponent *m_transform;

    mth::Box3 m_box;
    glm::vec3 m_color;

  public:
    BoxHelper(mth::Box3 t_box, glm::vec3 t_color = rend::Colors::red);

    void init_points();

    void update(double) override {}

    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override{};

    void upate(mth::Box3 t_box)
    {
        m_box = t_box;
        
        m_transform->transform.set_position(m_box.center());
        m_transform->transform.set_scale(m_box.size() * 0.5f);
    }
};


using BoxHelperPtr = std::unique_ptr<BoxHelper>;

template<typename... T>
inline BoxHelperPtr box_helper(T &&... args)
{
    return std::make_unique<BoxHelper>(args...);
}


}  // namespace ay::gmt

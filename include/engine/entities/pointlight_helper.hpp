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


class PointlightHelper : public Entity
{
  private:

    cmp::MeshComponent *m_mesh;
    cmp::TransformComponent *m_transform;
    
    float m_size; 
    glm::vec3 m_color;

  public:
    PointlightHelper(float size = 4.0f, glm::vec3 t_color = rend::Colors::red);

    void init_points();

    void update(double) override {}

    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override{};

};


using PointLightHelperPtr = std::unique_ptr<PointlightHelper>;

template<typename... T>
inline PointLightHelperPtr pointlight_helper(T &&... args)
{
    return std::make_unique<PointlightHelper>(args...);
}


}  // namespace ay::gmt

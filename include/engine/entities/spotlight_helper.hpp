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

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::gmt
{


class SpotlightHelper : public Entity
{
  private:
    cmp::LineSegmentsComponent *m_line_segments;
    cmp::TransformComponent *m_transform;

    glm::vec3 m_color;
    glm::vec3 m_dir;

    float m_distance{50.0f};
    float m_angle{mth::PI/4};
    

  public:
    SpotlightHelper(float t_distance = 50.0f,
                    float t_angle = mth::PI/8,
                    glm::vec3 dir  = {0.0f, -1.0f, 0.0f},
                    glm::vec3 color  = { 0.7f, 0.7f, 0.7f });

    void init_points();

    void update(double) override
    {
    }

    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override{};

    void update(float t_distance, float t_angle);
    
    void dir(glm::vec3 t_dir);


  private:

    void update_tranform();
    
};


using SpotlightHelperPtr = std::unique_ptr<SpotlightHelper>;

template<typename... T>
inline SpotlightHelperPtr spotlight_helper(T &&... args)
{
    return std::make_unique<SpotlightHelper>(args...);
}


}  // namespace ay::gmt

#pragma once

#include "application/event.hpp"

#include "engine/game_base.hpp"
#include "engine/component.hpp"
#include "engine/entity_factory.hpp"

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
    AxisHelper();

    void init_meshes();
    
    void update(double) override
    {}

    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override
    {}
    
};


using AxisHelperPtr = std::unique_ptr<AxisHelper>;


inline AxisHelperPtr axis()
{
    return std::make_unique<AxisHelper>();
}


}  // namespace ay::gmt

#pragma once

#include "engine/game_base.hpp"
#include "graphics/line_segments.hpp"

#include "engine/entity.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "macros.hpp"

namespace ay::cmp
{

class LineSegmentsComponent : public gmt::Component
{
  public:
    grph::LineSegments segments;

    using construct_type = grph::LineSegments;
    
  public:
    AY_COMPONENT(LineSegments)

    LineSegmentsComponent(grph::LineSegments t_segments)
      : segments(std::move(t_segments)){};

    void init(gmt::GameBase *t_game) override
    {
        segments.material()->init_shader(t_game->shaders());
    }
};

inline grph::LineSegments &line_segments(gmt::Entity *t_entity)
{
    auto p = t_entity->component<LineSegmentsComponent>();
    if (!p)
    {
    }
    return p->segments;
}

}  // namespace ay::cmp

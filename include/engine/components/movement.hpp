#pragma once

#include "engine/game_base.hpp"
#include "graphics/mesh.hpp"

#include "engine/entity.hpp"
#include "engine/components/transform.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "macros.hpp"




namespace ay::cmp
{


class MovementComponent : public gmt::Component
{
  public:

    mth::Transform& transform;
    
  public:
    AY_COMPONENT(Movement)

    MovementComponent(mth::Transform& t_transform) : transform(t_transform)
    {

    }

    void init(gmt::GameBase*) override
    {        
    }


    bool event(app::Event &) override
    {

    }

    void update(double) override
    {
    }
};



}

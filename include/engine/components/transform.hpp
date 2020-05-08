#pragma once

#include "engine/entity.hpp"
#include "math/transform.hpp"

#include "macros.hpp"
#include "std_header.hpp"

namespace ay::cmp
{

class TransformComponent : public gmt::Component
{
  public:

    using construct_type = mth::Transform;
    
    mth::Transform transform;

    AY_COMPONENT(Tranform)

    TransformComponent() : transform(){};

    TransformComponent(glm::vec3 t_position, glm::quat t_rotation, glm::vec3 t_scale)
      : transform(t_position, t_rotation, t_scale){};

    TransformComponent(mth::Transform t_transform) : transform(t_transform){};
};


inline mth::Transform &transform(gmt::Entity *t_entity)
{
    auto p = t_entity->component<TransformComponent>();
    if (!p)
    {
    }
    return p->transform;
}

}  // namespace ay::cmp

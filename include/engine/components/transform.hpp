#pragma once

#include "engine/entity.hpp"
#include "math/transform.hpp"

#include "macros.hpp"
#include "std_header.hpp"

namespace ay
{

class TransformComponent : public Component
{
  public:
    Transform transform;

  public:
    AY_COMPONENT(Tranform)

    TransformComponent() : transform(){};

    TransformComponent(glm::vec3 t_position, glm::quat t_rotation, glm::vec3 t_scale)
      : transform(t_position, t_rotation, t_scale){};

    TransformComponent(Transform t_transform) : transform(t_transform){};
};


inline Transform &transform(Entity *t_entity)
{
    auto p = t_entity->component<TransformComponent>();
    if (!p)
    {
    }
    return p->transform;
}

}  // namespace ay

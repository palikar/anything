#pragma once


#include "commons.hpp"
#include "std_header.hpp"
#include "macros.hpp"

#include "math/transform.hpp"

#include "graphics/mesh.hpp"
#include "application/event.hpp"


namespace ay
{

struct ComponentType {
    std::string name;
};

class Entity;

class Component
{
    
  protected:
    Entity* m_entity;
    
  public:

    virtual Entity* entity() { return m_entity; };
    virtual ComponentType* type() const { return nullptr; }
    virtual void update(double) {}
    virtual bool event(Event&) {return false; }

};

using ComponentPtr = std::unique_ptr<Component>;

class TransformComponent : public Component
{
  public:
    Transform transform;
  public:
    AY_COMPONENT(Tranform)
    
    TransformComponent() : transform()
    {};

    TransformComponent(glm::vec3 t_position, glm::quat t_rotation, glm::vec3 t_scale) :
        transform(t_position,t_rotation, t_scale)
    {};
    
    TransformComponent(Transform t_transform) : transform(t_transform)
    {};
};

class MeshComponent : public Component
{
  public:
    Mesh mesh;
  public:
    AY_COMPONENT(Mesh)
    
    MeshComponent(Mesh t_mesh) : mesh(std::move(t_mesh))
    {};
};


}

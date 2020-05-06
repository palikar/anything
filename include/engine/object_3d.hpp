#pragma once

#include "graphics/material.hpp"
#include "graphics/geometry.hpp"

#include "engine/entity.hpp"
#include "engine/component.hpp"
#include "engine/game_base.hpp"
#include "engine/entity_factory.hpp"

#include "engine/components/group.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/transform.hpp"
#include "engine/components/model.hpp"
#include "engine/components/name.hpp"


#include "std_header.hpp"
#include "glm_header.hpp"
#include "math_header.hpp"

namespace ay::gmt
{


class Object3D : public Entity
{
  private:
    cmp::TransformComponent *m_transform;
    cmp::GroupComponent *m_children;
    cmp::ModelComponent *m_model;
    cmp::NameComponent *m_name;

  public:
    Object3D(std::string name = "")
    {
        m_transform = add_component<cmp::TransformComponent>();
        m_children  = add_component<cmp::GroupComponent>();
        m_model     = add_component<cmp::ModelComponent>(grph::Model({}));
        m_name      = add_component<cmp::NameComponent>(std::move(name));
    }


    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override
    {
    }

    void update(double) override
    {
        m_transform->transform.update();
    }

    mth::Transform &transform()
    {
        return m_transform->transform;
    }

    const glm::vec3 &position() const
    {
        return m_transform->transform.position();
    }

    const glm::quat &rotation() const
    {
        return m_transform->transform.rotation();
    }

    const glm::vec3 &scale() const
    {
        return m_transform->transform.scale();
    }

    void translateX(float value)
    {
        m_transform->transform.translateX(value);
    }

    void translateY(float value)
    {
        m_transform->transform.translateY(value);
    }

    void translateZ(float value)
    {
        m_transform->transform.translateZ(value);
    }

    void rotateX(float value)
    {
        m_transform->transform.rotateX(value);
    }

    void rotateY(float value)
    {
        m_transform->transform.rotateY(value);
    }

    void rotateZ(float value)
    {
        m_transform->transform.rotateZ(value);
    }

    void look_at(glm::vec3 pos)
    {
        m_transform->transform.look_at(pos, { 0.0f, 1.0f, 0.0f });
    }

    glm::vec3 up() const
    {
        return glm::vec3(0.0, 1.0, 0.0);
    }

    std::vector<EntityPtr> &children()
    {
        return m_children->children;
    }

    bool frustum_culled() const
    {
        return false;
    }

    glm::mat4 &matrix()
    {
        return m_transform->transform.transform();
    }

    const std::string &name() const
    {
        return m_name->name;
    }

    void attach(EntityPtr obj)
    {
        m_children->children.push_back(std::move(obj));
    }

    void add_mesh(grph::Mesh mesh)
    {
        m_model->model.add_mesh(std::move(mesh));
    }

    grph::Mesh &mesh(size_t index = 0)
    {
        return *m_model->model.get(index);
    }

    template<typename T>
    T &material(size_t index = 0)
    {
        return *m_model->model.get(index)->material<T>();
    }

    Entity *child(size_t index = 0)
    {
        return m_children->children[index].get();
    }
};

using ObjectPtr = std::unique_ptr<Object3D>;

inline ObjectPtr object_3d(std::string name)
{
    return std::make_unique<Object3D>(std::move(name));
}

inline ObjectPtr object_mesh(grph::Mesh mesh)
{
    auto n = std::make_unique<Object3D>();
    n->add_mesh(std::move(mesh));
    return n;
}

template<typename... T>
inline ObjectPtr object_mesh(T &&... mesh)
{
    auto n = std::make_unique<Object3D>();

    (n->add_mesh(std::move(mesh)), ...);

    return n;
}


}  // namespace ay::gmt

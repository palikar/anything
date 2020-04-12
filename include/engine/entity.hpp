#pragma once

#include "std_header.hpp"

#include "application/event.hpp"

#include "engine/component.hpp"

#include "graphics/mesh.hpp"

namespace ay
{

class Entity
{

  protected:
    std::unordered_map<ComponentType*, ComponentPtr> m_components;
    std::string m_id;

    bool m_update_components{true};
    bool m_enabled{true};
    bool m_render{false};

    void update_components(double dt) {
        for (auto& [_, comp] : m_components) {
            comp->update(dt);
        }
    }

  public:
    Entity(std::string t_id = {}) : m_id(t_id) {}

    virtual ~Entity(){}

    template <typename T>
    const T* component() const
    {
        return component_internal<T>();
    }

    template <typename T>
    T* component()
    {
        return component_internal<T>();
    }

    void add_component(ComponentPtr t_comp)
    {
        m_components.insert({t_comp->type(), std::move(t_comp)});
    }

    virtual void update(double dt) {
        if (m_update_components) {
            update_components(dt);
        }
    }
    virtual bool event(Event&) { return false;}
    
    bool enabled() { return m_enabled; }
    bool renderable() { return m_enabled; }

  private:

    template <typename T>
    T* component_internal()
    {
        ComponentType* type = T::static_type();
        auto it = m_components.find(type);
        if (it == m_components.end())
            return nullptr;
        return static_cast<T*>(it->second.get());
    }

};

using EntityPtr = std::unique_ptr<Entity>;


inline Transform& get_transform(Entity* t_entity) {
    auto p = t_entity->component<TransformComponent>();
    if (!p)
    {}
    return p->transform;
}

inline Mesh& get_mesh(Entity* t_entity) {
    auto p = t_entity->component<MeshComponent>();
    if (!p)
    {}
    return p->mesh;
}

}

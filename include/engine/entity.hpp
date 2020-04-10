#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <memory>


#include "engine/component.hpp"
#include "graphics/mesh.hpp"

namespace ay
{

class Entity {
    
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
    
  private:

    template <typename T>
    T* component_internal()
    {
        ComponentType* type = T::static_type();
        auto it = m_components.find(type);
        if (it == m_components.end())
            return nullptr;
        return it->second.get();
    }

};

using EntityPtr = std::unique_ptr<Entity>;



inline EntityPtr mesh_entity(Mesh mesh, std::string name = {})
{
    EntityPtr new_ent = std::make_unique<Entity>(name);
    new_ent->add_component(std::make_unique<TransformComponent>());
    new_ent->add_component(std::make_unique<MeshComponent>(std::move(mesh)));
    return new_ent;
}



}
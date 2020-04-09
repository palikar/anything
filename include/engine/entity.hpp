#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <memory>


#include "engine/component.hpp"

namespace ay
{

class Entity {
    
  protected:
    std::unordered_map<ComponentType*, ComponentPtr> m_components;
    std::string m_id;

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

    virtual void update(double) {}

    
  private:

    template <typename T>
    T* component_internal()
    {
        ComponentType* type = T::GetStaticType();
        auto it = m_components.find(type);
        if (it == m_components.end())
            return nullptr;
        return it->second.get();
    }

};

using EntityPtr = std::unique_ptr<Entity>;

}

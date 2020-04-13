#pragma once


#include "application/event.hpp"
#include "engine/component.hpp"

#include "std_header.hpp"

namespace ay
{

class GameBase;

class Entity
{

  protected:
    std::unordered_map<ComponentType *, ComponentPtr> m_components;
    std::string m_id;
    GameBase *m_game{ nullptr };

    bool m_update_components{ true };
    bool m_enabled{ true };
    bool m_render{ false };

    void update_components(double dt)
    {
        for (auto &[_, comp] : m_components)
        {
            comp->update(dt);
        }
    }

  public:
    Entity(std::string t_id = {}) : m_id(t_id)
    {
    }

    virtual ~Entity()
    {
    }

    template<typename T>
    const T *component() const
    {
        return component_internal<T>();
    }

    template<typename T>
    T *component()
    {
        return component_internal<T>();
    }

    void add_component(ComponentPtr t_comp)
    {
        if (m_game != nullptr)
        {
            t_comp->init(m_game);
        }

        m_components.insert({ t_comp->type(), std::move(t_comp) });
    }

    void set_game(GameBase *t_game)
    {
        m_game = t_game;
        for (auto &[t, comp] : m_components)
        {
            comp->init(m_game);
        }
    };

    GameBase *game()
    {
        return m_game;
    };


    virtual void update(double dt)
    {
        if (m_update_components)
        {
            update_components(dt);
        }
    }
    virtual bool event(Event &)
    {
        return false;
    }
    virtual void init(GameBase *){};


    bool enabled()
    {
        return m_enabled;
    }
    bool renderable()
    {
        return m_enabled;
    }

  private:
    template<typename T>
    T *component_internal()
    {
        ComponentType *type = T::static_type();
        auto it             = m_components.find(type);
        if (it == m_components.end())
            return nullptr;
        return static_cast<T *>(it->second.get());
    }
};

using EntityPtr = std::unique_ptr<Entity>;

}  // namespace ay

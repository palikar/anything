#pragma once


#include "application/event.hpp"
#include "engine/component.hpp"

#include "std_header.hpp"

namespace ay::gmt
{

class GameBase;

class Entity
{

  protected:
    std::unordered_map<ComponentType *, ComponentPtr> m_components;
    uint32_t m_id;
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
    Entity(uint32_t t_id = {}) : m_id(t_id)
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

    template<typename T>
    auto add_component(std::unique_ptr<T> t_comp)
    {
        if (m_game != nullptr)
        {
            t_comp->init(m_game);
        }

        return static_cast<T *>(
          m_components.insert({ t_comp->type(), std::move(t_comp) }).first->second.get());
    }


    template<typename T>
    auto add_component()
    {
        auto t_comp = std::make_unique<T>();

        if (m_game != nullptr)
        {
            t_comp->init(m_game);
        }

        return static_cast<T *>(
          m_components.insert({ t_comp->type(), std::move(t_comp) }).first->second.get());
    }

    template<typename T, typename... Args>
    auto add_component(Args &&... args)
    {
        auto t_comp =
          std::make_unique<T>(typename T::construct_type(std::forward<Args>(args)...));

        if (m_game != nullptr)
        {
            t_comp->init(m_game);
        }

        return static_cast<T *>(
          m_components.insert({ t_comp->type(), std::move(t_comp) }).first->second.get());
    }

    void set_game(GameBase *t_game, uint32_t t_id)
    {
        m_game = t_game;
        m_id   = t_id;

        for (auto &[t, comp] : m_components)
        {
            comp->set_game(m_game, t_id + (m_components.size() + (1 << 16)));
            comp->init(m_game);
        }
    };

    GameBase *game()
    {
        return m_game;
    };

    uint32_t id() const
    {
        return m_id;
    }


    virtual void update(double dt)
    {
        if (m_update_components)
        {
            update_components(dt);
        }
    }
    virtual bool event(app::Event &)
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

}  // namespace ay::gmt

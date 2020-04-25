#pragma once


#include "application/event.hpp"

#include "std_header.hpp"


namespace ay::gmt
{

struct ComponentType
{
    std::string name;
};

class Entity;
class GameBase;

class Component
{

  protected:
    Entity *m_entity;
    GameBase *m_game;

    uint32_t m_id;

  public:
    void set_game(GameBase *t_game, uint32_t t_id)
    {
        m_game = t_game;
        m_id   = t_id;
    }


    uint32_t id()
    {
        return m_id;
    }

    virtual ~Component()
    {
    }

    virtual Entity *entity()
    {
        return m_entity;
    };

    virtual ComponentType *type() const
    {
        return nullptr;
    }

    virtual void update(double)
    {
    }

    virtual bool event(app::Event &)
    {
        return false;
    }

    virtual void init(GameBase *)
    {
    }
};

using ComponentPtr = std::unique_ptr<Component>;


}  // namespace ay::gmt

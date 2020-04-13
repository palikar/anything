#pragma once


#include "application/event.hpp"

#include "std_header.hpp"


namespace ay
{

struct ComponentType {
    std::string name;
};

class Entity;
class GameBase;

class Component
{

  protected:
    Entity* m_entity;

  public:
    virtual ~Component(){}

    virtual Entity* entity() { return m_entity; };
    virtual ComponentType* type() const { return nullptr; }
    virtual void update(double) {}
    virtual bool event(Event&) {return false; }
    virtual void init(GameBase*) {}

};

using ComponentPtr = std::unique_ptr<Component>;




}

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <memory>



namespace ay
{

struct ComponentType {
    std::string name;
};

class Entity;

class Component {
    
  protected:
    Entity* m_entity;
    
  public:

    virtual Entity* entity() { return m_entity; };
    virtual ComponentType* type() const { return nullptr; }
    virtual void update(double dt) {}

};

using ComponentPtr = std::unique_ptr<Component>;



}

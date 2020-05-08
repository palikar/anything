#pragma once

#include "engine/particle_system/particle.hpp"

#include "engine/entity.hpp"
#include "engine/component.hpp"
#include "engine/game_base.hpp"

namespace ay::cmp
{

class ParticleComponent : public gmt::Component
{
  public:
    using construct_type = typename std::unique_ptr<part::ParticleSystemBase>;

    std::unique_ptr<part::ParticleSystemBase> system;

    AY_COMPONENT(Particle)

    ParticleComponent(std::unique_ptr<part::ParticleSystemBase> t_system) : system(std::move(t_system)){};

    void init(gmt::GameBase *t_game) override
    {
        system->material()->make_instanced();
        system->material()->init_shader(t_game->shaders());
        
    }

    void update(double dt)
    {
        system->update(static_cast<float>(dt));
    }

    
};

inline part::ParticleSystemBase &particle_system(gmt::Entity *t_entity)
{
    auto p = t_entity->component<ParticleComponent>();
    if (!p)
    {
    }
    return *p->system;
}

}

#pragma once

#include "engine/game_base.hpp"

#include "engine/entity.hpp"
#include "engine/animation/animator.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "macros.hpp"

namespace ay::cmp
{

enum class AnimStateChange
{
    PLAY = 0,
    STOP,
    RESET,
    PAUSE
};

class AnimationComponent : public gmt::Component
{

  public:
    anim::Animator animator;

  private:
    using anim_event = std::tuple<std::string, AnimStateChange, KeyCode>;

    std::vector<anim_event> m_press_animation;
    std::vector<anim_event> m_release_animation;

    void process_event(std::vector<anim_event> &t_events, KeyCode t_code)
    {

        for (auto &[name, state, code] : t_events)
        {
            if (code != t_code)
            {
                return;
            }
            switch (state)
            {
            case AnimStateChange::PLAY: animator.play(name);

            case AnimStateChange::PAUSE: animator.pause(name);

            case AnimStateChange::STOP: animator.stop(name);

            case AnimStateChange::RESET: animator.reset(name);
            }
        }
    }

    void key_press(app::KeyPressedEvent ev)
    {
        process_event(m_press_animation, ev.key_code());
    }

    void key_release(app::KeyReleasedEvent ev)
    {
        process_event(m_release_animation, ev.key_code());
    }

  public:
    AY_COMPONENT(Animation)

    AnimationComponent()
    {
    }

    void init(gmt::GameBase *) override
    {
    }

    void update(double dt) override
    {
        animator.update(static_cast<float>(dt));
    }

    bool event(app::Event &e) override
    {
        app::Dispatcher dispatch{ e };
        dispatch.dispatch<app::KeyPressedEvent>(MEMBER(key_press));
        dispatch.dispatch<app::KeyReleasedEvent>(MEMBER(key_release));
        return false;
    }

    void add_key_press_change(std::string name, AnimStateChange state, KeyCode key)
    {
        m_press_animation.push_back({ std::move(name), state, key });
    }

    void add_key_release_change(std::string name, AnimStateChange state, KeyCode key)
    {
        m_release_animation.push_back({ std::move(name), state, key });
    }
};

inline anim::Animator &animation(gmt::Entity *t_entity)
{
    auto p = t_entity->component<AnimationComponent>();
    if (!p)
    {
    }
    return p->animator;
}

}  // namespace ay::cmp

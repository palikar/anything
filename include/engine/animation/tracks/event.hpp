#pragma once

#include "engine/animation/track.hpp"

namespace ay::anim
{


class EventTrack : public BaseTrack
{

  protected:
    std::vector<std::function<void()>> m_events;

    size_t m_current = 0;

  public:

    EventTrack(std::vector<std::function<void()>> events, std::vector<float> times)
        : BaseTrack(std::move(times)), m_events(std::move(events))
    {
    }

    void update_time(float t) override
    {
        if (before_start(t))
        {
            return;
        }

        if (m_current < m_times.size() && m_times[m_current] < t)
        {
            std::invoke(m_events[m_current]);
            ++m_current;
        }
    }

    void reset() override
    {
        m_current = 0;
    }

};

}  // namespace ay::anim

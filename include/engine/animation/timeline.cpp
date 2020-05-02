#include "timeline.hpp"


namespace ay::anim
{

void Timeline::update(float dt)
{
    m_current_time += dt;

    if (m_current_time > m_max_time)
    {
        if (m_parameters.m_mode == LoopMode::ONCE)
        {
            m_playing = false;
            return;
        }
        else if (m_parameters.m_mode == LoopMode::REPEAT)
        {
            m_current_time = 0.0f;
        }
    }

    for (auto &[_, track] : m_tracks)
    {
        track->update_time(m_current_time);
    }
}

void Timeline::reset()
{
    m_current_time = 0.0f;
}

bool Timeline::playing() const
{
    return m_playing;
}

TimelineParameters &Timeline::parameters()
{
    return m_parameters;
}

}

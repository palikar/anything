#include "timeline.hpp"


namespace ay::anim
{

void Timeline::update(float dt)
{
    
    if (m_current_time == 0.0)
    {
        run_start_callbacks();
    }
    
    m_current_time += dt * m_parameters.m_time_damping;

    if (m_current_time > m_max_time)
    {
        run_end_callbacks();
        if (m_parameters.m_mode == LoopMode::ONCE)
        {
            m_playing = false;
            if (m_parameters.m_reset_on_end)
            {
                reset();
            }
            return;
        }
        else if (m_parameters.m_mode == LoopMode::REPEAT)
        {
            reset();
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
    for (auto &[_, track] : m_tracks)
    {
        track->reset();
    }
}

bool Timeline::playing() const
{
    return m_playing;
}

TimelineParameters &Timeline::parameters()
{
    return m_parameters;
}

void Timeline::add_on_end_callback(std::function<void()> fun)
{
    m_end_callbacks.push_back(std::move(fun));
}

void Timeline::add_on_start_callback(std::function<void()> fun)
{
    m_start_callbacks.push_back(std::move(fun));
}

void Timeline::run_start_callbacks()
{
    for (auto& call : m_start_callbacks)
    {
        call();
    }
}

void Timeline::run_end_callbacks()
{
    for (auto& call : m_end_callbacks)
    {
        call();
    }
}



}

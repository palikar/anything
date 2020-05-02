

#include "animator.hpp"


namespace ay::anim
{



Animator::Animator()
{
    add_timeline("main_timeline", anim::Timeline());
}

Timeline &Animator::timeline(const std::string &t_name)
{
    return m_timelines.at(t_name).timeline;
}

void Animator::add_timeline(std::string t_timeline,
                            std::string t_track_name,
                            BaseTrackPtr t_track)
{
    m_timelines.at(t_timeline)
      .timeline.push_track(std::move(t_track_name), std::move(t_track));
}


Timeline &Animator::main_timeline()
{
    return m_timelines.at("main_timeline").timeline;
}

void Animator::add_timeline(std::string t_name, Timeline t_line)
{
    m_timelines.insert(
        { std::move(t_name), TimelineObject{ std::move(t_line), false } });
}

void Animator::update(float dt)
{
    for (auto &[name, timeline] : m_timelines)
    {
        timeline.timeline.update(dt);
        timeline.playing = timeline.timeline.playing();
    }
}

void Animator::play_all()
{
    for (auto &[name, timeline] : m_timelines)
    {
            timeline.playing = true;
        }
    }

void Animator::pause_all()
{
    for (auto &[name, timeline] : m_timelines)
    {
        timeline.playing = false;
    }
}

void Animator::reset_all()
{
    for (auto &[name, timeline] : m_timelines)
    {
        timeline.timeline.reset();
    }
}

void Animator::stop_all()
{
    for (auto &[name, timeline] : m_timelines)
        {
            timeline.playing = false;
            timeline.timeline.reset();
        }
    }


void Animator::play(const std::string &t_name)
{
    m_timelines.at(t_name).playing = true;
}

void Animator::pause(const std::string &t_name)
{
    m_timelines.at(t_name).playing = false;
}

void Animator::reset(const std::string &t_name)
{
    m_timelines.at(t_name).timeline.reset();
}

void Animator::stop(const std::string &t_name)
{
        m_timelines.at(t_name).playing = false;
        m_timelines.at(t_name).timeline.reset();
    }

}

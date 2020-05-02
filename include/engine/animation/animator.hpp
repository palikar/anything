#pragma once


#include "track.hpp"
#include "timeline.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"
#include "math_header.hpp"

namespace ay::anim
{


class Animator
{
    struct TimelineObject
    {
        Timeline timeline;
        bool playing{ false };
    };

  private:
    std::unordered_map<std::string, TimelineObject> m_timelines;

  public:
    Animator() = default;

    Timeline &timeline(const std::string &t_name)
    {
        return m_timelines.at(t_name).timeline;
    }

    void add_timeline(std::string t_name, Timeline t_line)
    {
        m_timelines.insert(
          { std::move(t_name), TimelineObject{ std::move(t_line), false } });
    }

    void update(float dt)
    {
        for (auto &[name, timeline] : m_timelines)
        {
            timeline.timeline.update(dt);
            timeline.playing = timeline.timeline.playing();
        }
    }

    void play_all()
    {
        for (auto &[name, timeline] : m_timelines)
        {
            timeline.playing = true;
        }
    }

    void pause_all()
    {
        for (auto &[name, timeline] : m_timelines)
        {
            timeline.playing = false;
        }
    }

    void reset_all()
    {
        for (auto &[name, timeline] : m_timelines)
        {
            timeline.timeline.reset();
        }
    }

    void stop_all()
    {
        for (auto &[name, timeline] : m_timelines)
        {
            timeline.playing = false;
            timeline.timeline.reset();
        }
    }


    void play(const std::string &t_name)
    {
        m_timelines.at(t_name).playing = true;
    }

    void pause(const std::string &t_name)
    {
        m_timelines.at(t_name).playing = false;
    }

    void reset(const std::string &t_name)
    {
        m_timelines.at(t_name).timeline.reset();
    }

    void stop(const std::string &t_name)
    {
        m_timelines.at(t_name).playing = false;
        m_timelines.at(t_name).timeline.reset();
    }
};


}  // namespace ay::anim

#pragma once


#include "track.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"
#include "math_header.hpp"

namespace ay::anim
{

enum class LoopMode
{
    ONCE = 0,
    REPEAT,
    PING_PONG
};

struct TimelineParameters
{
    LoopMode m_mode{ LoopMode::REPEAT };
};

class Timeline
{
  private:
    float m_current_time{ 0.0f };
    float m_max_time{ 0.0f };
    bool m_playing{ false };

    TimelineParameters m_parameters;

    std::unordered_map<std::string, BaseTrackPtr> m_tracks;

  public:
    void update(float dt);

    template<typename T>
    T *track(const std::string &t_name)
    {
        return static_cast<T *>(m_tracks.at(t_name).get());
    }

    template<typename T>
    T *push_track(std::string name, std::unique_ptr<T> t_track)
    {
        m_max_time = std::max(t_track->max_time(), m_max_time);
        return static_cast<T *>(
          m_tracks.insert({ std::move(name), std::move(t_track) }).first->second.get());
    }

    void reset();

    bool playing() const;

    TimelineParameters &parameters();

    
};


}  // namespace ay::anim

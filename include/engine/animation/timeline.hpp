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


class Timeline
{
  private:
    float m_current_time{0.0f};
    float m_max_time{10.0f};

    bool m_playing{false};

    LoopMode m_mode{LoopMode::ONCE};

    std::vector<BaseTrack> m_tracks;

  public:



    void update(float dt)
    {
        m_current_time += dt;

        if (m_current_time > m_max_time)
        {
            if (m_mode == LoopMode::ONCE) {
                m_playing = false;
            } else if (m_mode == LoopMode::REPEAT) {
                m_current_time = 0.0f;
            }
        }
        
    }

    void reset()
    {
        m_current_time = 0.0f;
    }

    bool playing() const {
        return m_playing;
    }

};



}

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
    Animator();

    Timeline &timeline(const std::string &t_name);

    void add_timeline(std::string t_timeline,
                      std::string t_track_name,
                      BaseTrackPtr t_track);

    Timeline &main_timeline();

    void add_timeline(std::string t_name, Timeline t_line);

    void update(float dt);

    void play_all();

    void pause_all();

    void reset_all();

    void stop_all();

    void play(const std::string &t_name = "main_timeline");

    void pause(const std::string &t_name = "main_timeline");

    void reset(const std::string &t_name = "main_timeline");

    void stop(const std::string &t_name = "main_timeline");
};


}  // namespace ay::anim

#pragma once

#include "glm_header.hpp"
#include "std_header.hpp"
#include "math_header.hpp"

namespace ay::anim
{

class BaseTrack
{

  protected:
    std::vector<float> m_times;

    std::array<std::pair<float, int>, 2> get_times(float time)
    {
        for (size_t i = 1; i < m_times.size(); ++i)
        {
            if (time <= m_times[i])
            {
                return { std::pair{ m_times[i - 1], i - 1 }, std::pair{ m_times[i], i } };
            }
        }
        return {};
    }

    bool before_start(float time)
    {
        return time < m_times.front();
    }

    bool after_end(float time)
    {
        return m_times.back() < time;
    }

  public:

    virtual void update_time(float t) = 0;

    virtual void reset() {};

    virtual ~BaseTrack()
    {
    }

    BaseTrack(std::vector<float> times) : m_times(std::move(times))
    {
    }

    float max_time()
    {
        return *std::max_element(m_times.begin(), m_times.end());
    }


};

using BaseTrackPtr = std::unique_ptr<BaseTrack>;

template<typename T, typename Function>
class Track : public BaseTrack
{

  public:
    using fun_type = Function *;

    using ptr_type = std::unique_ptr<Track<T, Function>>;

    using data_type = T;

    using ref_type = typename std::vector<T>::const_reference;

    static Function *fun;

  protected:

    std::vector<T> m_values;
    
    T m_current;

    T *m_target{ nullptr };

    mth::EasingFun m_easing{};

    T interpolate(std::pair<ref_type, ref_type> values, float factor)
    {

        return fun(values.first, values.second, m_easing(factor));

    }
  public:


    Track(std::vector<T> values, std::vector<float> times)
      : BaseTrack(std::move(times)), m_values(std::move(values))
    {
    }

    void update_time(float t) override
    {
        if (before_start(t) or after_end(t))
        {
            return;
        }

        const auto times = get_times(t);
        const auto p = std::make_pair(m_values[times[1].second], m_values[times[0].second]);


        const float time = (times[1].first - t) / (times[1].first - times[0].first);

        m_current = interpolate(p, time);

        if (m_target != nullptr)
        {
            *m_target = m_current;
        }
    }

    void set_target(T *value)
    {
        m_target = value;
    }

    T &current()
    {
        return m_current;
    }

    mth::EasingFun& easing()
    {
        return m_easing;
    }
};


}  // namespace ay::anim

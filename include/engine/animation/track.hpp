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

    std::array<std::pair<float, int>, 2> get_times(float)
    {

        return {std::pair{0.1f, 0}, std::pair{0.10f, 1}};
    }

  public:

    virtual void update_time(float t) = 0;

    BaseTrack(std::vector<float> times) : m_times(std::move(times))
    {

    }
};


template<typename T, typename Function>
class Track : public BaseTrack
{
  private:
    
  protected:
    std::vector<T> m_values;

    T m_current;
    T* m_target;

    T interpolate(std::pair<T&,T&> values, float factor)
    {
        return fun(values.first, values.second, factor);
    }

  public:
    
    using fun_type = Function*;
    static Function *fun;

    void update_time(float t) override
    {
        auto times = get_times(t);
        auto p     = std::make_pair(std::ref(m_values[times[0].second]),
                                    std::ref(m_values[times[1].second]));
        float time = (times[1].first - t) / (times[1].first - times[0].first);

        m_current = interpolate(p, time);
        
    }
    

    Track(std::vector<T> values, std::vector<float> times)
        : BaseTrack(std::move(times)), m_values(std::move(values))
    {
    }

    T &current()
    {
        return m_current;
    }

};


static glm::vec3 inter(glm::vec3 a, glm::vec3 b, float f)
{
    return a*(1-f) * b*f; 
}

using Vec3Track = anim::Track<glm::vec3, decltype(inter)>;
template<>Vec3Track::fun_type Vec3Track::fun = &inter;




}

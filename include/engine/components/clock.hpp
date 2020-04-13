#pragma once


#include "engine/component.hpp"

#include "macros.hpp"
#include "std_header.hpp"

namespace ay::cmp
{


class Clock : public gmt::Component
{
  private:
    float m_time = 0.0f;
    float m_running{ false };

    std::vector<std::tuple<double, double, std::function<void()>>> m_callbacks;

  public:
    AY_COMPONENT(Clock)

    Clock()
    {
    }

    void start()
    {
        m_running = true;
        m_time    = 0;
    }

    void stop()
    {
        m_running = false;
    }

    float elapsed_time()
    {
        return static_cast<float>(m_time);
    }

    void update(double dt) override
    {
        if (!m_running)
        {
            return;
        }

        m_time += dt;
        for (auto &[acc, period, call] : m_callbacks)
        {
            acc += dt;
            if (acc >= period)
            {
                call();
                acc = 0;
            }
        }
    }

    template<typename T>
    void add_callback(double period, T &&t_callback)
    {
        m_callbacks.push_back({ 0.0, period, std::forward<T>(t_callback) });
    }
};

}  // namespace ay::cmp

#pragma once

#include "std_header.hpp"
#include "glm_header.hpp"
#include "math_header.hpp"

#include <variant>

#include "util/random.hpp"


namespace ay::part
{

using Parameter = std::variant<float, glm::vec2, glm::vec3, glm::vec4>;
using Position  = glm::vec3;
using Color     = glm::vec4;

using EmitterParameters = std::unordered_map<std::string, Parameter>;

class ParticleEmitter
{
  protected:
    EmitterParameters m_parameters;


  public:
    ParticleEmitter()                 = default;
    virtual glm::vec3 next_position() = 0;
    virtual glm::vec3 next_velocity() = 0;

    virtual void update_parameters() = 0;

    virtual EmitterParameters &parameters()
    {
        return m_parameters;
    }

    template<typename T>
    T &parameter(const std::string &name)
    {
        return std::get<T>(m_parameters[name]);
    }
};


class LineEmitter : public ParticleEmitter
{
  private:
    glm::vec3 m_p1;
    glm::vec3 m_p2;
    glm::vec3 m_vel;

  public:
    LineEmitter(glm::vec3 p1, glm::vec3 p2) : m_p1(p1), m_p2(p2), m_vel(0.0, 1.0, 0.0)
    {
        m_parameters["start"]    = p1;
        m_parameters["end"]      = p2;
        m_parameters["velocity"] = m_vel;
    }

    void update_parameters() override
    {
        m_p1  = std::get<Position>(m_parameters["start"]);
        m_p2  = std::get<Position>(m_parameters["end"]);
        m_vel = std::get<Position>(m_parameters["velocity"]);
    }

    glm::vec3 next_position() override
    {
        auto t = util::Random::uniform_real(0, 1.0f);
        return (1 - t) * m_p1 + t * m_p2;
    }

    glm::vec3 next_velocity() override
    {
        return m_vel;
    }
};

class CircleEmitter : public ParticleEmitter
{
  private:
    glm::vec3 m_p;
    float m_radius;
    glm::vec3 m_vel;

  public:
    CircleEmitter(glm::vec3 p = { 0.0, 0.0, 0.0 }, float radius = 1.0)
      : m_p(p), m_radius(radius), m_vel(0.0, 1.0, 0.0)
    {
        m_parameters["position"] = p;
        m_parameters["radius"]   = radius;
        m_parameters["velocity"] = m_vel;
    }

    void update_parameters() override
    {
        m_p      = std::get<Position>(m_parameters["position"]);
        m_radius = std::get<float>(m_parameters["radius"]);
        m_vel    = std::get<Position>(m_parameters["velocity"]);
    }

    glm::vec3 next_position() override
    {
        auto t = util::Random::uniform_real(0, 2 * mth::PI);
        return m_p + glm::vec3{ m_radius * std::sin(t), 0.0, m_radius * std::cos(t) };
    }

    glm::vec3 next_velocity() override
    {
        return m_vel;
    }
};

class CircleAreaEmitter : public ParticleEmitter
{
  private:
    glm::vec3 m_p;
    float m_max_radius;
    glm::vec3 m_vel;

  public:
    CircleAreaEmitter(glm::vec3 p = { 0.0, 0.0, 0.0 }, float radius = 1.0)
      : m_p(p), m_max_radius(radius), m_vel(0.0f, 1.0f, 0.0f)
    {
        m_parameters["position"] = p;
        m_parameters["radius"]   = radius;
        m_parameters["velocity"] = m_vel;
    }

    void update_parameters() override
    {
        m_p          = std::get<Position>(m_parameters["position"]);
        m_max_radius = std::get<float>(m_parameters["radius"]);
        m_vel        = std::get<Position>(m_parameters["velocity"]);
    }

    glm::vec3 next_position() override
    {
        auto t = util::Random::uniform_real(0, 2 * mth::PI);
        auto r = util::Random::uniform_real(0, m_max_radius);
        return m_p + glm::vec3{ r * std::sin(t), 0.0, r * std::cos(t) };
    }

    glm::vec3 next_velocity() override
    {
        return m_vel;
    }
};

class ConeEmitter : public ParticleEmitter
{

  private:
    glm::vec3 m_p;
    float m_angle;

  public:
    ConeEmitter(glm::vec3 p = { 0.0, 0.0, 0.0 }, float angle = 1.0)
      : m_p(p), m_angle(angle)
    {
        m_parameters["position"] = p;
        m_parameters["angle"]    = angle;
    }

    void update_parameters() override
    {
        m_p     = std::get<Position>(m_parameters["position"]);
        m_angle = std::get<float>(m_parameters["angle"]);
    }

    glm::vec3 next_position() override
    {
        return m_p;
    }

    glm::vec3 next_velocity() override
    {
        const auto polar   = util::Random::uniform_real(-m_angle / 2.0f, m_angle / 2.0f);
        const auto azimuth = util::Random::uniform_real(-m_angle / 2.0f, m_angle / 2.0f);

        const auto pos = glm::vec3{ std::sin(polar) * std::cos(azimuth),
                                    std::sin(polar) * std::sin(azimuth),
                                    std::cos(polar) };

        return m_p + pos;
    }
};
}  // namespace ay::part

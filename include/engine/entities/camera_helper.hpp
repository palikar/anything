#pragma once


#include "application/event.hpp"

#include "engine/game_base.hpp"
#include "engine/component.hpp"
#include "engine/entity.hpp"

#include "graphics/material.hpp"
#include "graphics/materials/solid_color.hpp"


#include "engine/components/line_segments.hpp"
#include "engine/components/transform.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::gmt
{


class CameraHelper : public Entity
{
  private:
    std::unordered_map<std::string, std::vector<size_t>> m_points_map;
    std::vector<float> *m_point_data;

    cmp::LineSegmentsComponent *m_line_segments;
    cmp::TransformComponent *m_transform;

    glm::mat4 m_proj_mat;
    glm::mat4 m_proj_mat_inv;

    void add_point(std::string a);

    void set_point(const std::string &a, glm::vec3 value);

    void add_line(const std::string &a, const std::string &b);

  public:
    CameraHelper(glm::mat4 t_proj_mat);

    void init_points();

    void update(double dt) override;

    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override{};
};


}  // namespace ay::gmt

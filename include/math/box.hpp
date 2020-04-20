#pragma once


#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::mth
{

class Box3
{
  private:
    glm::vec3 m_min;
    glm::vec3 m_max;

  public:
    Box3(glm::vec3 t_min = {}, glm::vec3 t_max = {}) : m_min(t_min), m_max(t_max)
    {
    }


    void set(glm::vec3 t_min = {}, glm::vec3 t_max = {})
    {
        m_min = t_min;
        m_max = t_max;
    }

    glm::vec3 &min()
    {
        return m_min;
    }

    glm::vec3 &max()
    {
        return m_max;
    }


    Box3 &set_from_buffer_attribute(std::vector<float> attribute)
    {

        float minX = std::numeric_limits<float>::max();
        float minY = std::numeric_limits<float>::max();
        float minZ = std::numeric_limits<float>::max();

        float maxX = std::numeric_limits<float>::min();
        float maxY = std::numeric_limits<float>::min();
        float maxZ = std::numeric_limits<float>::min();

        for (size_t i = 0, l = attribute.size(); i < l; i++)
        {

            float x = attribute[i];
            float y = attribute[i + 1];
            float z = attribute[i + 2];

            if (x < minX)
                minX = x;
            if (y < minY)
                minY = y;
            if (z < minZ)
                minZ = z;

            if (x > maxX)
                maxX = x;
            if (y > maxY)
                maxY = y;
            if (z > maxZ)
                maxZ = z;
        }

        m_min = { minX, minY, minZ };
        m_max = { maxX, maxY, maxZ };

        return *this;
    }

    Box3 &set_from_center_and_size(glm::vec3 center, float size)
    {
        auto halfSize = glm::vec3(size, size, size) * 0.5f;

        m_min = center - halfSize;
        m_max = center + halfSize;

        return *this;
    }

    glm::vec3 center() const
    {
        return (m_max + m_min) * 0.5f;
    }

    glm::vec3 size() const
    {
        return (m_max - m_min);
    }

    bool contains_point(glm::vec3 point)
    {

        return point.x < m_min.x || point.x > m_max.x || point.y < m_min.y
                   || point.y > m_max.y || point.z < m_min.z || point.z > m_max.z
                 ? false
                 : true;
    }

    bool contains_box(Box3 box)
    {

        return m_min.x <= box.m_min.x && box.m_max.x <= m_max.x && m_min.y <= box.m_min.y
               && box.m_max.y <= m_max.y && m_min.z <= box.m_min.z
               && box.m_max.z <= m_max.z;
    }

    bool intersects_box(Box3 box)
    {

        return box.m_max.x < m_min.x || box.m_min.x > m_max.x || box.m_max.y < m_min.y
                   || box.m_min.y > m_max.y || box.m_max.z < m_min.z
                   || box.m_min.z > m_max.z
                 ? false
                 : true;
    }

    Box3 &make_union(Box3 box)
    {

        m_min = glm::min(m_min, box.m_min);
        m_max = glm::max(m_max, box.m_max);


        return *this;
    }

    Box3 &translate(glm::vec3 offset)
    {

        m_min += offset;
        m_max += offset;

        return *this;
    }

    Box3 &expand_by_point(glm::vec3 point)
    {

        m_min = glm::min(m_min, point);
        m_max = glm::max(m_max, point);
        return *this;
    }

    Box3 &expand_by_vec(glm::vec3 vec)
    {

        m_min -= vec;
        m_max += vec;
        return *this;
    }

    Box3 &expand_by_scaler(float scale)
    {

        m_min -= scale;
        m_max += scale;
        return *this;
    }
};

}  // namespace ay::mth

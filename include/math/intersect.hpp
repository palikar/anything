#pragma once


#include "math/box.hpp"
#include "math/line.hpp"
#include "math/plane.hpp"
#include "math/ray.hpp"
#include "math/sphere.hpp"
#include "math/utils.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::mth
{

inline std::optional<glm::vec3> intersect_ray(const Ray& ray, const Sphere &sphere)
{

    const auto v        = sphere.center() - ray.origin();
    const float tca     = glm::dot(v, ray.dir());
    const float d2      = glm::dot(v, v) - (tca * tca);
    const float radius2 = sphere.radius() * sphere.radius();
    if (d2 > radius2)
        return {};

    const float thc = std::sqrt(radius2 - d2);
    const float t0  = tca - thc;
    const float t1 = tca + thc;

    if (t0 < 0 && t1 < 0)
        return {};

    if (t0 < 0)
        return ray.at(t1);

    return ray.at(t0);
}

inline bool intersects_ray(const Ray &ray, const Sphere &sphere)
{
    return ray.distance_sq_to_point(sphere.center())
           <= (sphere.radius() * sphere.radius());
}

inline std::optional<glm::vec3> intersect_ray(const Ray &ray, const Plane &plane)
{

    // plane.distance_sq_to_point(ray.origin());
    float dist = 0;
    float d    = glm::dot(plane.normal(), ray.dir());
    if (d == 0.0)
    {
        if (plane.distance_to_point(ray.origin()) == 0)
        {
            dist = 0;
        }
        return {};
    }
    else
    {
        dist = -(glm::dot(ray.origin(), plane.normal()) + plane.constant()) / d;
    }

    return dist > 0 ? std::optional(ray.at(dist)) : std::nullopt;
}

inline bool intersects_ray(const Ray &ray, const Plane &plane)
{
    return intersect_ray(ray, plane).has_value();
}

inline std::optional<glm::vec3> intersect_ray(const Ray &ray, const Box3 &box)
{

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    const float invdirx = 1 / ray.dir().x;
    const float invdiry = 1 / ray.dir().y;
    const float invdirz = 1 / ray.dir().z;

    const auto origin = ray.origin();

    if (invdirx >= 0)
    {

        tmin = (box.min().x - origin.x) * invdirx;
        tmax = (box.max().x - origin.x) * invdirx;
    }
    else
    {

        tmin = (box.max().x - origin.x) * invdirx;
        tmax = (box.min().x - origin.x) * invdirx;
    }

    if (invdiry >= 0)
    {

        tymin = (box.min().y - origin.y) * invdiry;
        tymax = (box.max().y - origin.y) * invdiry;
    }
    else
    {

        tymin = (box.max().y - origin.y) * invdiry;
        tymax = (box.min().y - origin.y) * invdiry;
    }

    if ((tmin > tymax) || (tymin > tmax))
        return {};

    if (tymin > tmin || tmin != tmin)
        tmin = tymin;

    if (tymax < tmax || tmax != tmax)
        tmax = tymax;

    if (invdirz >= 0)
    {

        tzmin = (box.min().z - origin.z) * invdirz;
        tzmax = (box.max().z - origin.z) * invdirz;
    }
    else
    {

        tzmin = (box.max().z - origin.z) * invdirz;
        tzmax = (box.min().z - origin.z) * invdirz;
    }

    if ((tmin > tzmax) || (tzmin > tmax))
        return {};

    if (tzmin > tmin || tmin != tmin)
        tmin = tzmin;

    if (tzmax < tmax || tmax != tmax)
        tmax = tzmax;

    if (tmax < 0)
        return {};

    return ray.at(tmin >= 0 ? tmin : tmax);
}

inline bool intersects_box(const Ray &ray, const Box3 &box)
{
    return intersect_ray(ray, box).has_value();
}

inline std::optional<glm::vec3> intersect_ray(const Ray &ray,
                                              glm::vec3 a,
                                              glm::vec3 b,
                                              glm::vec3 c,
                                              bool backfaceCulling = false)
{

    auto edge1        = (b - a);
    auto edge2        = (c - a);
    const auto normal = glm::cross(edge1, edge2);

    float DdN = glm::dot(normal, ray.dir());
    float sign;

    if (DdN > 0)
    {

        if (backfaceCulling)
            return {};
        sign = 1;
    }
    else if (DdN < 0)
    {

        sign = -1;
        DdN  = -DdN;
    }
    else
    {
        return {};
    }

    const auto diff = (ray.origin() - a);
    edge1           = glm::cross(diff, edge2);

    float DdQxE2 = sign * glm::dot(ray.dir(), edge1);

    if (DdQxE2 < 0)
    {
        return {};
    }

    float DdE1xQ = sign * glm::dot(ray.dir(), glm::cross(edge1, diff));

    if (DdE1xQ < 0)
    {
        return {};
    }

    if (DdQxE2 + DdE1xQ > DdN)
    {
        return {};
    }

    float QdN = -sign * glm::dot(diff, normal);

    if (QdN < 0)
    {
        return {};
    }

    return ray.at(QdN / DdN);
}


}  // namespace ay::mth

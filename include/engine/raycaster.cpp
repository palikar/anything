#include "engine/raycaster.hpp"





#include "engine/camera.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/transform.hpp"

#include "math_header.hpp"
#include "glm_header.hpp"

#include "application/input.hpp"

namespace ay::gmt
{

mth::Ray Raycaster::camera_to_mouse()
{


    auto coord = app::Input::mouse_pos();
    

    const glm::vec3 norm_coord{ (2.0f * coord.x) / viewport_width - 1.0f,
                                1.0 - (2.0f * coord.y) / viewport_height,
                                1.0f };

    const glm::vec4 clip_coord{ norm_coord.x, norm_coord.y , -1.0f, 1.0f };


    auto temp = glm::inverse(camera->projection()) * clip_coord;

    const glm::vec4 eye_coord{ temp.x, temp.y, -1.0f, 0.0f};

    temp = glm::inverse(camera->view()) * eye_coord;

    const glm::vec3 world_coord{ temp.x,  temp.y, temp.z};

    return mth::Ray{ camera->pos(), glm::normalize(world_coord) };
}

mth::Ray Raycaster::from_position(glm::vec2 pos)
{

    const glm::vec3 norm_coord{ pos.x, pos.y, 1.0f };

    const glm::vec4 clip_coord{ glm::xy(norm_coord), -1.0f, 1.0f };

    const glm::vec4 eye_coord{ glm::xy(glm::inverse(camera->projection()) * clip_coord),
                               -1.0f,
                               0.0f };
    const glm::vec3 world_coord{ glm::xyz(glm::inverse(camera->view()) * eye_coord) };

    return mth::Ray{ camera->pos(), glm::normalize(world_coord) };
}

mth::Ray Raycaster::from_position(glm::vec2 pos, glm::mat4 proj, glm::mat4 view)
{

    const glm::vec3 norm_coord{ pos.x, pos.y, 1.0f };

    const glm::vec4 clip_coord{ glm::xy(norm_coord), -1.0f, 1.0f };

    const glm::vec4 eye_coord{ glm::xy(glm::inverse(proj) * clip_coord), -1.0f, 0.0f };

    const glm::vec3 world_coord{ glm::xyz(glm::inverse(view) * eye_coord) };

    return mth::Ray{ camera->pos(), glm::normalize(world_coord) };
}

std::pair<Entity*, std::vector<Entity*>> Raycaster::intersect_objects(std::vector<EntityPtr> &t_objs)
{
    auto ray = camera_to_mouse();

    float current_dist = std::numeric_limits<float>::max();
    size_t index = 0;
    size_t i = 0;
    std::vector<Entity*> objs;
    for (auto& obj : t_objs)
    {
        ++i;
            
        const auto& mesh_cmp = obj->component<cmp::MeshComponent>();
        const auto& transform_cmp = obj->component<cmp::TransformComponent>();

        if (!mesh_cmp or !transform_cmp) { continue; }

        const auto& transform = transform_cmp->transform.transform();
        const auto& sphere = mesh_cmp->mesh.geometry().bounding_sphere();

        const auto local_ray = ray.transform(glm::inverse(transform));
        if (auto p = mth::intersect_ray(local_ray, sphere)) {
            float new_dist = glm::distance( p.value(), local_ray.origin());

            objs.push_back(obj.get());
                    
            if (new_dist >= current_dist) { continue; }

            current_dist = new_dist;
            index = i;
                
        }            
    }
        
    return std::make_pair(t_objs.at(index).get(), objs);

}

}  // namespace ay::gmt

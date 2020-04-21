#include "engine/raycaster.hpp"


namespace ay::gmt
{

mth::Ray Raycaster::camera_to_mouse()
{


    auto coord = app::Input::mouse_pos();

    const glm::vec3 norm_coord{ ((2.0f * coord.x) / viewport_width) - 1.0f,
                                ((2.0f * coord.y) / viewport_height) - 1.0f,
                                1.0f };


    const glm::vec4 clip_coord{ glm::xy(norm_coord), -1.0f, 1.0f };

    const glm::vec4 eye_coord{ glm::xy(glm::inverse(camera->projection()) * clip_coord),
                               -1.0f,
                               0.0f };

    const glm::vec3 world_coord{ glm::xyz(glm::inverse(camera->view()) * eye_coord) };

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


}  // namespace ay::gmt

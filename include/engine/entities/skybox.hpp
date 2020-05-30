#pragma once

#include "application/event.hpp"

#include "engine/game_base.hpp"
#include "engine/component.hpp"
#include "engine/entity.hpp"

#include "graphics/geometry_factory.hpp"

#include "rendering/shaders.hpp"
#include "rendering/cube_texture.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::gmt
{


class Skybox : public Entity
{
  private:
    grph::Geometry m_cube;
    rend::CubeTexturePtr m_texture;
    rend::ShaderPtr m_shader;

  public:
    Skybox(rend::CubeTexturePtr tex, size_t t_size = 500)
    {

        m_cube = grph::cube_geometry(t_size, t_size, t_size);
        // m_cube.drop_attribute("normal");
        // m_cube.drop_attribute("uv");
        m_cube.pack();

        m_texture = std::move(tex);
    }

    virtual void init(GameBase *t_game)
    {
        m_shader = t_game->shaders().load("skybox");
    };

    rend::Shader *shader() const
    {
        return m_shader.get();
    }

    rend::CubeTexture *texture()
    {
        return m_texture.get();
    }

    const rend::VertexArray *buffers() const
    {
        return m_cube.gl_buffers();
    }

    grph::Geometry &geometry()
    {
        return m_cube;
    }
};

using SkyboxPtr = std::unique_ptr<Skybox>;

template<typename... Args>
SkyboxPtr skybox(Args &&... args)
{
    return std::make_unique<Skybox>(std::forward<Args>(args)...);
}

}  // namespace ay::gmt

#include "engine/game_base.hpp"
#include "engine/engine.hpp"

namespace ay::gmt
{


gmt::TextureLibrary &GameBase::textures()
{
    return m_engine->tex_lib();
}

Scene3D *GameBase::init_scene(const std::string &t_name)
{
    auto sc = Scene3D();
    sc.init(this);
    return &m_scenes.insert({ t_name, std::move(sc) }).first->second;
}

gmt::ShaderLibrary &GameBase::shaders()
{
    return m_engine->shader_lib();
}


void GameBase::set_up(GameEngine *t_engine)
{
    m_engine = t_engine;
}


}  // namespace ay::gmt

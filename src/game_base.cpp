#include "engine/game_base.hpp"
#include "engine/engine.hpp"

namespace ay
{


ShaderLibrary& GameBase::shaders()
{
    return m_engine->shader_lib();
}


void GameBase::set_up(GameEngine* t_engine){
    m_engine = t_engine;
    
}



}

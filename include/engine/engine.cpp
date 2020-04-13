
#include "engine/engine.hpp"

#include "engine/game_base.hpp"

namespace ay::gmt
{

GameEngine::GameEngine(gmt::GameBase *t_game) : m_game(t_game), m_renderAPI()
{
}

void GameEngine::init(int width, int height)
{
    m_width  = width;
    m_height = height;

    m_renderAPI.init();
    m_renderAPI.set_viewport(0, 0, m_width, m_height);
    m_renderAPI.set_clear_color(glm::vec4(0.0, 0.0, 0.0, 0.0));

    m_game->set_up(this);

    m_game->init();
}

void GameEngine::update(double dt)
{
    m_game->update(dt);
}

void GameEngine::render()
{
    m_renderAPI.clear();
    m_game->render(m_renderAPI);
}

bool GameEngine::resize(app::WindowResizeEvent &e)
{
    m_renderAPI.set_viewport(0, 0, e.width(), e.height());
    m_width  = e.width();
    m_height = e.height();
    return true;
}

bool GameEngine::on_event(app::Event &t_event)
{
    return m_game->event(t_event);
}

}  // namespace ay::gmt

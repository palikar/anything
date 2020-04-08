#pragma once




namespace ay {



class Component {
    
  private:


  public:

};



class Entity {
    
  private:


  public:

};

class GameEngine;
class Renderer;


class GameBase {
    
  protected:
    GameEngine* m_engine;
    
  public:


    void set_up(GameEngine* t_engine);

    virtual void init() = 0;
    virtual void update(double dt) = 0;
    virtual void render(Renderer& renderer) = 0;

};





}

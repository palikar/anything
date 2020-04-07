#pragma once

#include "application/resource_loader.hpp"
#include "rendering/buffers.hpp"
#include "rendering/vertex_array.hpp"
#include "rendering/shaders.hpp"
#include "math/transform.hpp"
#include "commons.hpp"

#include <GL/glew.h>
#include <iostream>

namespace ay {


class GameBase;


class GameEngine {

  private:
    int m_width;
    int m_height;

    ShaderLibrary m_shader_lib;

  public:
    explicit GameEngine(GameBase*){

    }

    void init(int width, int height) {
        m_width = width;
        m_height = height;

        m_shader_lib.load("simple");
    }

    void update(double) {

    }


    void render() {
 

        


    }





};

}

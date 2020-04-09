#include <iostream>
#include <memory>
#include <utility>

#include <cmath>

#include "ay.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace ay;


class SimpleGame : public ay::GameBase {

  private:

    ay::VertexArrayPtr buffers;
    float acc = 0;
    Camera camera;
    Transform model;

  public:
    
    SimpleGame()
    {

    }

    void init() override
    {
        buffers = make_vertex_array(
            make_index_buffer({
                    {0, 1, 2},
                    {2, 3, 0},
            
                    {1, 5, 6},
                    {6, 2, 1},
            
                    {7, 6, 5},
                    {5, 4, 7},
            
                    {4, 0, 3},
                    {3, 7, 4},
            
                    {4, 5, 1},
                    {1, 0, 4},
                
                    {3, 2, 6},
                    {6, 7, 3}
                }),
            make_vertex_buffer({
                    {-1.0, -1.0,  1.0},
                    {1.0, -1.0,  1.0},
                    {1.0,  1.0,  1.0},
                    {-1.0,  1.0,  1.0},

                    {-1.0, -1.0, -1.0},
                    {1.0, -1.0, -1.0},
                    {1.0,  1.0, -1.0},
                    {-1.0,  1.0, -1.0}
                }));
        
        auto s = m_engine->shader_lib().load("simple");
        
        
        model.set_scale(glm::vec3(2,2,2));
        model.set_position(glm::vec3(0, 0, 0));

        camera.init_orthographic(100, -100, 100, -100, -100, 100);
        camera.init_prescpective_projection(glm::radians(45.0f), 1024.0/768.0, 0.001f, 10000.0f);

    }

    float deg = 1;

    void update(double) override
    {
        acc += 0.01;
        deg += 0.01;

        const float radius = 10.0f;
        float camX = sin(deg) * radius;
        float camZ = cos(deg) * radius;
        
        camera.set_look_at(glm::vec3(camX, 10, camZ), glm::vec3(0,0,0));
        
        auto s = shaders().get("simple");
        s->set("projection_matrix", camera.view_projection());
        s->set("model_matrix", model.get_tranformation());
    }

    
    void render(ay::Renderer& rend) override
    {
        auto s = shaders().bind("simple");
        buffers->bind();

        // s->set("red_channel", 0.0f);
        // rend.draw_indexed(buffers);
        // s->set("red_channel", 1.0f);
        
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        rend.draw_indexed(buffers.get());
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        
    }


};




int main()
{

    std::unique_ptr<SimpleGame> game = std::make_unique<SimpleGame>();
    
    ay::Application app{1024, 768, game.get()};

    return app.run();



}

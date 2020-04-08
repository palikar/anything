
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

        camera.init_prescpective_projection(glm::radians(90.0f), 4.0 / 3.0, 0.001f, 10000.0f);
        camera.init_orthographic(10, -10, 10, -10, -1, 3);
        
        
        buffers = std::make_shared<ay::VertexArray>();

        // float verts[] = {
        //     0.5f,   0.5f,  0.00, 
        //     0.5f,  -0.5f,  0.00, 
        //     -0.5f, -0.5f,  0.00,
        //     -0.5f,  0.5f,  0.00            
        // };

        
        float verts[] = {
            .5f,   .5f,  0.0, 
            .5f,  -.5f,  0.0, 
            -.5f, -.5f,  0.0,
            -.5f,  .5f,  0.0            
        };

        uint32_t ind[] = {
            0, 1, 3,
            1, 2, 3
        };
        
        auto buf = std::make_shared<VertexBuffer>(verts, sizeof(verts));
        buf->set_layout({{"position", ay::ShaderDataType::Float3}});

        auto index = std::make_shared<IndexBuffer>(ind, 6);


        buffers->add_vertex_buffer(buf);
        buffers->set_index_buffer(index);
        
        auto s = m_engine->shader_lib().load("simple");

        s->set("red_channel", 0.4f);
        s->set("projection_matrix", camera.view_projection());
        s->set("model_matrix", model.get_tranformation());

        
        model.set_scale(glm::vec3(5,5,5));
        
    }

    float deg = 1;

    void update(double) override
    {
        acc += 0.2;
        deg += 0.01;

        // camera.rotate(glm::vec3(0.0, 1.0, 0.0), glm::radians(0.1));
        
        model.set_position(glm::vec3(0, 0.1*std::sin(acc), 0));
        
        // model.move(glm::vec3(0.0, 1.0, 0.0), );
        // model.rotate(glm::vec3(0.0, 1.0, 0.0), glm::radians(1.0));

        if(Input::is_pressed(KeyCode::W)){
            camera.move(glm::mat3(camera.transform().rotation()) *  (glm::vec3(0.0, 1.0, 0.0)), .01);
        }

        if(Input::is_pressed(KeyCode::S)){
            camera.move(glm::mat3(camera.transform().rotation()) *  (glm::vec3(0.0, -1.0, 0.0)), .01);
        }
        
        if(Input::is_pressed(KeyCode::A)){
            camera.move(glm::mat3(camera.transform().rotation()) *  (glm::vec3(1.0, 0.0, 0.0)), .01);
        }

        if(Input::is_pressed(KeyCode::D)){
            camera.move(glm::mat3(camera.transform().rotation()) *  (glm::vec3(-1.0, 0.0, 0.0)), .01);
        }

        // if(Input::is_pressed(KeyCode::F)){
        //     camera.rotate(glm::vec3(0.0, 0.0, 1.0), .01);
        // }

        // if(Input::is_pressed(KeyCode::B)){
        //     camera.rotate(glm::vec3(0.0, 0.0, 1.0), -.01);
        // }

        // std::cout << "pos: " << glm::to_string(camera.tranform().position()) << "\n";
        
        auto s = m_engine->shader_lib().get("simple");
        s->set("red_channel", std::abs(std::sin(acc)));
        s->set("projection_matrix", camera.view_projection());
        s->set("model_matrix", model.get_tranformation());
        
    }

    
    void render(ay::Renderer& rend) override
    {
        m_engine->shader_lib().get("simple")->bind();
        buffers->bind();

        rend.draw_indexed(buffers);
        // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        // glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }


};




int main()
{

    std::unique_ptr<SimpleGame> game = std::make_unique<SimpleGame>();
    
    ay::Application app{1024, 768, game.get()};

    return app.run();



}
// * Anything

// ** Rendering API [0/2]
// - [ ] Factory methods 
//   - [ ] Factory methods for vertex arrays
//   - [ ] Factory methods for vertex buffers
//   - [ ] Factory methods for index buffers   
// - [ ] Vertex type
//   - [ ] Vertex array -> vertex buffer



// ** Application architecture [0/0]


// ** Engine architecture [0/1]

// - [ ] Event dispatching system
//   - [ ] Event types
//   - [ ] Event dispatcher

// - [ ] Better input system

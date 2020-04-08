#include <iostream>
#include <memory>
#include <utility>

#include <cmath>

#include "ay.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace ay;

template <typename T, int N>
constexpr int dim(T(&)[N])
{
    return N;
}


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

        // camera.init_orthographic(10, -10, 10, -10, -1, 3);
        
        
        
        buffers = std::make_shared<ay::VertexArray>();

        // float verts[] = {
        //     0.5f,   0.5f,  0.00, 
        //     0.5f,  -0.5f,  0.00, 
        //     -0.5f, -0.5f,  0.00,
        //     -0.5f,  0.5f,  0.00            
        // };

        
        float verts[] = {
            -1.0, -1.0,  1.0,
            1.0, -1.0,  1.0,
            1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            // back
            -1.0, -1.0, -1.0,
            1.0, -1.0, -1.0,
            1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0
        };

        uint32_t ind[] = {
            
            0, 1, 2,
            2, 3, 0,
            
            1, 5, 6,
            6, 2, 1,
            
            7, 6, 5,
            5, 4, 7,
            
            4, 0, 3,
            3, 7, 4,
            
            4, 5, 1,
            1, 0, 4,
            
            3, 2, 6,
            6, 7, 3
        };
        
        auto buf = std::make_shared<VertexBuffer>(verts, sizeof(verts));
        buf->set_layout({{"position", ay::ShaderDataType::Float3}});

        auto index = std::make_shared<IndexBuffer>(ind, dim(ind));


        buffers->add_vertex_buffer(buf);
        buffers->set_index_buffer(index);
        
        auto s = m_engine->shader_lib().load("simple");

        s->set("red_channel", 0.4f);
        s->set("projection_matrix", camera.view_projection());
        s->set("model_matrix", model.get_tranformation());

        
        model.set_scale(glm::vec3(2,2,2));
        model.set_position(glm::vec3(0, 0, 0));

        
        camera.init_prescpective_projection(glm::radians(45.0f), 1024.0/768.0, 0.001f, 10000.0f);

        // std::cout << glm::to_string(glm::axis(camera.rot())) << "\n";
        
        camera.set_position(glm::vec3(5,5,5));

        camera.set_rotation(glm::angleAxis(glm::radians(180.0f), glm::vec3(0,0,1)));
        // camera.set_rotation(glm::angleAxis(glm::radians(0.0f), glm::vec3(0,-,-1)));

        

        // std::cout << glm::to_string(glm::axis(camera.rot())) << "\n";
        // std::cout << glm::to_string(to_vec3(glm::conjugate(camera.rot()))) << "\n";

    }

    float deg = 1;

    void update(double dt) override
    {
        acc += 0.01;
        deg += 0.01;

        float cameraSpeed = 24.5f * dt;

        if(Input::is_pressed(KeyCode::W)){
            
            camera.move(forward(camera.rot()), cameraSpeed);
        }

        if(Input::is_pressed(KeyCode::S)){
            camera.move(backward(camera.rot()), cameraSpeed);
        }
        
        if(Input::is_pressed(KeyCode::D)){
            camera.move(glm::normalize(glm::cross(forward(camera.rot()),
                                                  glm::vec3(0,1,0))), cameraSpeed);            
        }

        if(Input::is_pressed(KeyCode::A)){

            camera.move(glm::normalize(glm::cross(glm::vec3(0,1,0),
                                                  forward(camera.rot()))), cameraSpeed);
        }

        auto proj = glm::perspective(glm::radians(45.0f), 1024.0f/768.0f, 0.001f, 10000.0f);
        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        glm::mat4 view;

        // camera.set_position(glm::vec3(camX, 0, camZ));
        // camera.set_rotation(glm::angleAxis(glm::radians(180.0f), glm::normalize(glm::vec3(camX, 0, camZ) - glm::vec3(0.0, 0.0, 0.0)) - glm::vec3(camX, 0, camZ)));
        
        // view = glm::lookAt(glm::vec3(camX, 0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        
        auto s = m_engine->shader_lib().get("simple");

        s->set("projection_matrix", camera.view_projection());
        s->set("model_matrix", model.get_tranformation());
        
        // s->set("projection_matrix", proj * view);
        // s->set("model_matrix", model.get_tranformation());

    }

    
    void render(ay::Renderer& rend) override
    {
        m_engine->shader_lib().get("simple")->bind();
        buffers->bind();

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        rend.draw_indexed(buffers);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        
    }


};




int main()
{

    std::unique_ptr<SimpleGame> game = std::make_unique<SimpleGame>();
    
    ay::Application app{1024, 768, game.get()};

    return app.run();



}

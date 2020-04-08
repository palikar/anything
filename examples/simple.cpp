
#include <iostream>
#include <memory>
#include <utility>

#include <cmath>

#include "ay.hpp"


using namespace ay;

class SimpleGame : public ay::GameBase {

  private:

    ay::VertexArrayPtr buffers;
    float acc = 0;

  public:
    
    SimpleGame()
    {

    }

    void init() override
    {
        std::cout << "Initing game" << "\n";

        buffers = std::make_shared<ay::VertexArray>();

        float verts[] = {
            0.5f,  0.5f, 0.0f, 
            0.5f, -0.5f, 0.0f, 
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
            
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

        s->set_float("red_channel", 0.4);

    }


    void update(double) override
    {
        acc += 0.2;
        m_engine->shader_lib().get("simple")->set_float("red_channel", std::abs(std::sin(acc)));
    }

    
    void render(ay::Renderer& rend) override
    {
        m_engine->shader_lib().get("simple")->bind();
        buffers->bind();
        rend.draw_indexed(buffers);
        
    }


};




int main()
{

    std::unique_ptr<SimpleGame> game = std::make_unique<SimpleGame>();
    
    ay::Application app{1024, 768, game.get()};

    return app.run();



}

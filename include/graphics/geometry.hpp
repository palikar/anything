#pragma once

#include "rendering/vertex.hpp"
#include "rendering/buffer_factory.hpp"


#include "glm_header.hpp"
#include "std_header.hpp"
#include "math_header.hpp"


namespace ay::grph
{

inline rend::ShaderDataType stride_to_data_type(size_t s)
{
    switch (s)
    {
    case 2: return rend::ShaderDataType::Float2;
    case 3: return rend::ShaderDataType::Float3;
    case 4: return rend::ShaderDataType::Float4;
    }
    return rend::ShaderDataType::None;
}

class Geometry
{

    struct Attriubute
    {
        std::vector<float> data;
        size_t stride;
        int max_size{-1};
        int buffer_index{-1};
    };

  private:
    std::vector<uint32_t> m_index;
    std::unordered_map<std::string, Attriubute> m_buffers;
    std::vector<std::tuple<int32_t, int32_t>> m_groups;
    rend::VertexArrayPtr m_glbuffers;

    bool m_dirty{ true };
    bool m_dynamic{ false };

  public:
    
    Geometry(bool t_dynamic = false) : m_dynamic(t_dynamic)
    {
    }

    auto &buffers()
    {
        return m_buffers;
    }

    void set_attribute(std::string name, std::vector<float> buffer, size_t stride)
    {
        m_buffers.insert({ std::move(name), { std::move(buffer), stride, -1, -1}});
    }

    void declare_attribute(std::string name, size_t stride, int max_size)
    {
        m_buffers.insert({ std::move(name), { {}, stride, max_size, -1}});
    }
    
    void drop_attribute(std::string name)
    {
        m_buffers.erase(m_buffers.find(name));
    }

    std::vector<float> &attribute(const std::string &name)
    {
        return (m_buffers.at(name)).data;
    }

    void set_index(std::vector<uint32_t> buffer)
    {
        m_index = std::move(buffer);
    }

    std::vector<uint32_t> &index()
    {
        return m_index;
    }

    void apply(glm::mat4 t_mat)
    {

        if (m_buffers.count("position"))
        {
            auto &pos = (m_buffers.at("position")).data;
            for (size_t i = 0; i < pos.size() - 2; i += 3)
            {
                auto res = t_mat * glm::vec4(pos[i], pos[i + 1], pos[i + 2], 1.0f);

                pos[i]     = res.x;
                pos[i + 1] = res.y;
                pos[i + 2] = res.z;
            }
        }

        if (m_buffers.count("normal"))
        {
            auto &norm      = (m_buffers.at("normal")).data;
            auto normal_mat = mth::normal(t_mat);
            for (size_t i = 0; i < norm.size() - 2; i += 3)
            {
                auto res = glm::vec3(norm[i], norm[i + 1], norm[i + 2]) * normal_mat;

                norm[i]     = res.x;
                norm[i + 1] = res.y;
                norm[i + 2] = res.z;
            }
        }

        if (m_buffers.count("tangent"))
        {
            auto &tan       = (m_buffers.at("normal")).data;
            auto normal_mat = mth::normal(t_mat);
            for (size_t i = 0; i < tan.size() - 2; i += 3)
            {
                auto res = glm::vec3(tan[i], tan[i + 1], tan[i + 2]) * normal_mat;
                res      = glm::normalize(res);

                tan[i]     = res.x;
                tan[i + 1] = res.y;
                tan[i + 2] = res.z;
            }
        }
    }

    void compute_bounding_box()
    {
    }

    void compute_bounding_sphere()
    {
    }

    void compute_vertex_normals()
    {
    }

    void look_at(glm::vec3 t_pos)
    {
        apply(glm::lookAt(glm::vec3(1, 0, 0), t_pos, glm::vec3(0, 1, 0)));
    }

    void merge(Geometry &other)
    {
        auto &oth   = other.m_buffers;
        auto offset = (m_buffers.at("position")).data.size() / 3;

        for (auto &[name, buff] : oth)
        {
            if (m_buffers.count(name) > 0)
            {
                if ((buff).stride != (m_buffers.at(name)).stride)
                {
                    continue;
                }

                auto &src    = (buff).data;
                auto &target = (m_buffers.at(name)).data;
                target.insert(target.end(), src.begin(), src.end());
            }
        }

        for (auto i : other.m_index)
        {
            m_index.push_back(i + offset);
        }
        m_dirty = true;
    }

    void normalize_normals()
    {
    }

    void rotate_x(float t_angle)
    {
        apply(glm::rotate(glm::mat4(1), t_angle, glm::vec3(1, 0, 0)));
    }

    void rotate_y(float t_angle)
    {
        apply(glm::rotate(glm::mat4(1), t_angle, glm::vec3(0, 1, 0)));
    }

    void rotate_z(float t_angle)
    {
        apply(glm::rotate(glm::mat4(1), t_angle, glm::vec3(0, 0, 1)));
    }

    void scale(float t_factor)
    {
        apply(glm::scale(glm::mat4(1), glm::vec3(1, 1, 1) * t_factor));
    }

    void translate(float t_x, float t_y, float t_z)
    {
        apply(glm::translate(glm::mat4(1), glm::vec3(t_x, t_y, t_z)));
    }

    void pack()
    {
        if (m_dynamic) {
            pack_vertex_buffers_dynamic();
        } else {
            pack_vertex_buffers();
        }

        if (m_groups.empty())
        {
            pack_group(0, m_index.size());
            return;
        }

        size_t last_start{0};
        size_t last_count{0};
        size_t i{ 0 };

        for (auto &[start, count] : m_groups)
        {
            pack_group(start, count, i++);
            last_start = start;
            last_count = count;
        }

        if (last_start + last_count < m_index.size())
        {
            pack_group(last_start + last_count,
                       m_index.size() - (last_start + last_count));
        }
        
        m_dirty = false;
    }

    void add_group(uint32_t start, uint32_t count)
    {
        m_groups.push_back({ start, count });
    }

    const rend::VertexArray *gl_buffers() const
    {
        return m_glbuffers.get();
    }

    rend::VertexArrayPtr take_buffers()
    {
        if (m_dirty)
        {
            pack();
        }
        m_dirty = true;
        return std::move(m_glbuffers);
    }

    bool is_dirty() const
    {
        return m_dirty;
    }

    bool indexed()
    {
        return !m_index.empty();
    }

  private:

    void pack_vertex_buffers()
    {
        m_glbuffers   = std::make_unique<rend::VertexArray>();

        if (m_buffers.count("position") > 0 && m_buffers.count("normal") > 0
            && m_buffers.count("uv") > 0)
        {
            
            
            auto &pos  = (m_buffers.at("position")).data;
            auto &norm = (m_buffers.at("normal")).data;
            auto &uv   = (m_buffers.at("uv")).data;
            std::vector<Vertex8fg> verts;

            for (size_t i = 0, j = 0; i < pos.size() - 2; i += 3, j += 2)
            {
                verts.push_back({ pos[i],
                                  pos[i + 1],
                                  pos[i + 2],
                                  norm[i],
                                  norm[i + 1],
                                  norm[i + 2],
                                  uv[j],
                                  uv[j + 1] });
            }

            m_glbuffers->add_vertex_buffer(rend::make_buffer(verts));
        }


        auto handle_attr = [&](const std::string &name) {
            auto buf    = m_buffers.at(name);
            auto &data  = (buf).data;
            auto stride = (buf).stride;
            
            const auto data_type = stride_to_data_type(stride);

            // std::cout << name << "\n";
            // for (size_t i = 0; i < data.size() - 2; ++i) {
            //     std::cout << data[i] << "," << data[i +1] << "," << data[i + 2] << "," << "\n";
            // }
            auto vert  = std::make_unique<rend::VertexBuffer>(data.data(),
                                                              data.size() * sizeof(float));
            
            vert->set_layout({ { name, data_type } });
            m_glbuffers->add_vertex_buffer(std::move(vert));
       };

        if (m_buffers.count("position") > 0)
        {
            handle_attr("position");
        }

        if (m_buffers.count("normal") > 0)
        {
            handle_attr("normal");
        }

        if (m_buffers.count("uv") > 0)
        {
            handle_attr("uv");
        }

        m_dirty = false;
    }

    void pack_vertex_buffers_dynamic()
    {
        if (!m_glbuffers){
            m_glbuffers   = std::make_unique<rend::VertexArray>();
        }

        if (m_buffers.count("position") > 0 && m_buffers.count("normal") > 0
            && m_buffers.count("uv") > 0)
        {

            auto &max_size = m_buffers.at("position").max_size;
            auto &pos  = (m_buffers.at("position")).data;
            auto &norm = (m_buffers.at("normal")).data;
            auto &uv   = (m_buffers.at("uv")).data;
            std::vector<float> verts;

            for (size_t i = 0, j = 0; i < pos.size() - 2; i += 3, j += 2)
            {
                verts.push_back( pos[i]);
                verts.push_back(pos[i + 1]);
                verts.push_back(pos[i + 2]);
                verts.push_back(norm[i]);
                verts.push_back(norm[i + 1]);
                verts.push_back(norm[i + 2]);
                verts.push_back(uv[j]);
                verts.push_back(uv[j + 1]);                
            }

            if(m_glbuffers->vertex_buffers().size() != 0) {
                auto buf = std::make_unique<rend::VertexBuffer>(max_size);

                buf->set_layout({ { "position", rend::ShaderDataType::Float3 },
                                  { "normal", rend::ShaderDataType::Float3 },
                                  { "uv", rend::ShaderDataType::Float2 }});
                
                buf->set_data(verts.data(), verts.size() * sizeof(float));
                m_glbuffers->add_vertex_buffer(std::move(buf));
            } else {
                m_glbuffers->vertex_buffers()[0]->set_data(verts.data(), verts.size() * sizeof(float));
            }
            
        }
        
        auto handle_attr = [&](const std::string &name) {
                               auto buf    = m_buffers.at(name);
                               auto &data  = (buf).data;
                               auto &stride = (buf).stride;
                               auto &size = (buf).max_size;
                               auto &buf_ind = (buf).buffer_index;
                               
            
                               const auto data_type = stride_to_data_type(stride);

                               if (buf_ind == -1) {
                                   auto gl_buf  = std::make_unique<rend::VertexBuffer>(size);
                                   buf_ind = m_glbuffers->vertex_buffers().size();
                                   gl_buf->set_layout({ { name, data_type } });
                                   gl_buf->set_data(data.data(), data.size() * sizeof(float));
                                   m_glbuffers->add_vertex_buffer(std::move(gl_buf));
                               } else {
                                   m_glbuffers->vertex_buffers()[buf_ind]->set_data(data.data(), data.size() * sizeof(float));
                               }
                               
                           };

        if (m_buffers.count("position") > 0)
        {
            handle_attr("position");
        }

        if (m_buffers.count("normal") > 0)
        {
            handle_attr("normal");
        }

        if (m_buffers.count("uv") > 0)
        {
            handle_attr("uv");
        }

        m_dirty = false;
    }

    void pack_group(uint32_t start, uint32_t count, size_t index = 0)
    {
        m_glbuffers->set_index_buffer(
          std::make_unique<rend::IndexBuffer>(m_index.data() + start, count), index);
    }
    
};


}  // namespace ay::grph

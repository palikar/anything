#include "graphics/geometry.hpp"


namespace ay::grph
{

void Geometry::apply(glm::mat4 t_mat)
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

void Geometry::merge(Geometry &other)
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

void Geometry::pack()
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

void Geometry::pack_vertex_buffers()
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


void Geometry::pack_vertex_buffers_dynamic()
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


void Geometry::pack_group(uint32_t start, uint32_t count, size_t index)
{
    m_glbuffers->set_index_buffer(
        std::make_unique<rend::IndexBuffer>(m_index.data() + start, count), index);
}

}

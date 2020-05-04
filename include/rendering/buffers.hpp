#pragma once

#include "util/gl_helpers.hpp"

#include "std_header.hpp"


namespace ay::rend
{

enum class ShaderDataType : uint16_t
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

inline uint32_t data_type_size(ShaderDataType t_type)
{
    switch (t_type)
    {
    case ShaderDataType::None: return 0;
    case ShaderDataType::Float: return 4;
    case ShaderDataType::Float2: return 4 * 2;
    case ShaderDataType::Float3: return 4 * 3;
    case ShaderDataType::Float4: return 4 * 4;
    case ShaderDataType::Mat3: return 4 * 3 * 3;
    case ShaderDataType::Mat4: return 4 * 4 * 4;
    case ShaderDataType::Int: return 4;
    case ShaderDataType::Int2: return 4 * 2;
    case ShaderDataType::Int3: return 4 * 3;
    case ShaderDataType::Int4: return 4 * 4;
    case ShaderDataType::Bool: return 1;
    }

    return 0;
}

inline uint32_t data_type_element_count(ShaderDataType t_type)
{
    switch (t_type)
    {
    case ShaderDataType::None: return 0;
    case ShaderDataType::Float: return 1;
    case ShaderDataType::Float2: return 2;
    case ShaderDataType::Float3: return 3;
    case ShaderDataType::Float4: return 4;
    case ShaderDataType::Mat3: return 3 * 3;
    case ShaderDataType::Mat4: return 4 * 4;
    case ShaderDataType::Int: return 1;
    case ShaderDataType::Int2: return 2;
    case ShaderDataType::Int3: return 3;
    case ShaderDataType::Int4: return 4;
    case ShaderDataType::Bool: return 1;
    }

    return 0;
}

inline GLenum data_type_gl_base_type(ShaderDataType t_type)
{
    switch (t_type)
    {
    case ShaderDataType::None: return 0;
    case ShaderDataType::Float: return GL_FLOAT;
    case ShaderDataType::Float2: return GL_FLOAT;
    case ShaderDataType::Float3: return GL_FLOAT;
    case ShaderDataType::Float4: return GL_FLOAT;
    case ShaderDataType::Mat3: return GL_FLOAT;
    case ShaderDataType::Mat4: return GL_FLOAT;
    case ShaderDataType::Int: return GL_INT;
    case ShaderDataType::Int2: return GL_INT;
    case ShaderDataType::Int3: return GL_INT;
    case ShaderDataType::Int4: return GL_INT;
    case ShaderDataType::Bool: return GL_BOOL;
    }

    return 0;
}

struct BufferElement
{
    std::string name;
    ShaderDataType type;
    bool per_instance{ false };
    size_t offset{ 0 };
    uint32_t size{ data_type_size(type) };
    bool normalized{ false };
};

class BufferLayout
{
  public:
    BufferLayout()
    {
    }
    BufferLayout(const std::initializer_list<BufferElement> &elements)
      : m_elements(elements)
    {
        calculate_offsets_and_stride();
    }

    auto &elements()
    {
        return m_elements;
    }
    const auto &elements() const
    {
        return m_elements;
    }

    void push_element(BufferElement t_element)
    {
        m_elements.push_back(std::move(t_element));
        calculate_offsets_and_stride();
    }

    uint32_t get_stride() const
    {
        return m_stride;
    }

  private:
    void calculate_offsets_and_stride()
    {
        size_t offset = 0;
        m_stride      = 0;
        for (auto &element : m_elements)
        {
            element.offset = offset;
            offset += element.size;
            m_stride += element.size;
        }
    }

    std::vector<BufferElement> m_elements;
    uint32_t m_stride = 0;
};

class VertexBuffer
{
  private:
    GLuint m_vbo;
    BufferLayout m_layout;

  public:
    VertexBuffer(uint32_t t_size)
    {
        glCreateBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, t_size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer(float *t_vertices, uint32_t t_size)
    {
        GLCall(glCreateBuffers(1, &m_vbo));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, t_size, t_vertices, GL_STATIC_DRAW));
    }

    ~VertexBuffer()
    {
        GLCall(glDeleteBuffers(1, &m_vbo));
    }

    void bind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    }

    void unbind()
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void set_data(const void *t_data, uint32_t t_size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, t_size, t_data);
    };

    const BufferLayout &get_layout() const
    {
        return m_layout;
    };

    void set_layout(BufferLayout t_layout)
    {
        m_layout = std::move(t_layout);
    };
};

class IndexBuffer
{
  private:
    GLuint m_vio;
    uint32_t m_count;

  public:
    IndexBuffer(uint32_t *indices, uint32_t count) : m_count(count)
    {
        GLCall(glCreateBuffers(1, &m_vio));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vio));
        GLCall(glBufferData(
          GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
    }

    ~IndexBuffer()
    {
        glDeleteBuffers(1, &m_vio);
    }

    void bind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vio));
    }

    void unbind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    uint32_t count() const
    {
        return m_count;
    }
};

using VertexBufferPtr = std::unique_ptr<VertexBuffer>;

using IndexBufferPtr = std::unique_ptr<IndexBuffer>;

}  // namespace ay::rend

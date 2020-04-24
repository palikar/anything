#pragma once

#include "rendering/buffers.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay
{

template<typename... T>
struct Types
{
};

template<rend::ShaderDataType... T>
struct ShaderTypes
{
};

template<size_t count, typename... T>
class Vertex3fGen;

template<size_t count, typename... T, rend::ShaderDataType... T_shad>
class Vertex3fGen<count, Types<T...>, ShaderTypes<T_shad...>>
{
  public:
    using data_type                       = std::common_type_t<T...>;
    constexpr static size_t element_size  = sizeof(data_type) * count;
    constexpr static size_t element_count = count;

    std::array<data_type, sizeof...(T)> elements;

    Vertex3fGen() = default;

    Vertex3fGen(T... vals) : elements{ vals... }
    {
    }

    template<size_t offset = 0, typename... El>
    void add_data(El... el)
    {
        size_t i = offset;
        ((elements[i++] = el), ...);
    }

    data_type &operator[](int index)
    {
        return elements[index];
    }

    data_type *data()
    {
        return elements.data();
    }
};


using Vertex3fg =
  Vertex3fGen<3, Types<float, float, float>, ShaderTypes<rend::ShaderDataType::Float3>>;

using Vertex6fg =
  Vertex3fGen<6,
              Types<float, float, float, float, float, float>,
              ShaderTypes<rend::ShaderDataType::Float3, rend::ShaderDataType::Float3>>;

using Vertex8fg =
  Vertex3fGen<8,
              Types<float, float, float, float, float, float, float, float>,
              ShaderTypes<rend::ShaderDataType::Float3,
                          rend::ShaderDataType::Float3,
                          rend::ShaderDataType::Float2>>;


class Vertex3f
{
  public:
    using data_type                       = float;
    constexpr static size_t element_size  = sizeof(data_type) * 3;
    constexpr static size_t element_count = 1;

    template<size_t index>
    constexpr size_t offset()
    {
        if constexpr (index == 0)
        {
            return offsetof(Vertex3f, position);
        }
        else
        {
            return 0;
        }
    }

    glm::vec3 position;

    Vertex3f() : position(0.0f, 0.0f, 0.0f)
    {
    }

    Vertex3f(float x, float y, float z) : position(x, y, z)
    {
    }

    float *data()
    {
        return glm::value_ptr(position);
    }
};

class Index3i
{
  public:
    uint32_t i_0;
    uint32_t i_1;
    uint32_t i_2;

    Index3i(size_t one, size_t two, size_t three) : i_0(one), i_1(two), i_2(three)
    {
    }


    Index3i() : i_0(0), i_1(0), i_2(0)
    {
    }
};


}  // namespace ay

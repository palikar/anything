#pragma once


#include "glm_header.hpp"

namespace ay
{

class Vertex3f
{
  public:

    using data_type = float;
    constexpr static size_t element_size = sizeof(data_type)*3;
    constexpr static size_t element_count = 1;

    template<size_t index>
    constexpr size_t offset() {
        if constexpr (index == 0) {
            return offsetof(Vertex3f, position);
        } else {
            return 0;
        }
    }
    
    glm::vec3 position;

    Vertex3f() : position(0.0f,0.0f,0.0f)
    {}

    Vertex3f(float x,float y,float z) : position(x,y,z)
    {}

    float* data() {
        return glm::value_ptr(position);
    }


};

class Index3i
{
  public:
    uint32_t i_0;
    uint32_t i_1;
    uint32_t i_2;

    Index3i(size_t one, size_t two, size_t three) :
        i_0(one),
        i_1(two),
        i_2(three)
    {}


    Index3i() : i_0(0), i_1(0), i_2(0)
    {}

    
};


}

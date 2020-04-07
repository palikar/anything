#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ay
{


inline glm::mat4 init_rotation(glm::vec3 forward, glm::vec3 up, glm::vec3 right){
    glm::mat4 m;

    m[0][0] = right.x;
    m[0][1] = right.y;
    m[0][2] = right.z;
    m[0][3] = 0;
    m[1][0] = up.x;
    m[1][1] = up.y;
    m[1][2] = up.z;
    m[1][3] = 0;
    m[2][0] = forward.x;
    m[2][1] = forward.y;
    m[2][2] = forward.z;
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
    
    return m;
}

inline glm::mat4 init_rotation(glm::vec3 forward, glm::vec3 up){
    auto f = glm::normalize(forward);
    auto r = glm::cross(f, glm::normalize(up));
    auto u = glm::cross(f, r);
    return init_rotation(f, u, r);
}

inline glm::mat4 init_scale(float x, float y, float z) {
    
    return glm::mat4(0,0,0,x,
                     0,0,0,y,
                     0,0,0,z,
                     0,0,0,1);
}

inline glm::mat4 init_scale(glm::vec3 v) {
    return init_scale(v.x,v.y,v.z);
}

inline glm::mat4 init_translate(float x, float y, float z) {
    
    return glm::mat4(x,0,0,0,
                     0,y,0,0,
                     0,0,z,0,
                     0,0,0,1);
}

inline glm::mat4 init_translate(glm::vec3 v) {
    return init_translate(v.x,v.y,v.z);
}

inline glm::mat4 init_identity() {

    return glm::mat4(1,0,0,0,
                     0,1,0,0,
                     0,0,1,0,
                     0,0,0,1);    
}

<template T>
inline T saturate(T x)
{
    if (x > 1.0d)
    {
        return 1.0d;
    }
    
    if (x < 0.0d)
    {
        return 0.0d;
    }
    
    return x;
}

}

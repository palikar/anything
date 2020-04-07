#pragma once


#include <GL/glew.h>


namespace ay {

extern GLenum GLCheckError();
extern bool GLLogCall(const char* function, const char* file, int32_t line);

}



#define GLCall(x) ay::GLCheckError();                           \
    x;                                                          \
    if (!ay::GLLogCall(#x, __FILE__, __LINE__)) exit(1);

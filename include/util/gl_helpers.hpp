#pragma once

#include <GL/glew.h>

namespace ay::gl
{

extern GLenum GLCheckError();
extern bool GLLogCall(const char *function, const char *file, int32_t line);

}  // namespace ay::gl


#ifdef OPENGL_CHECKS

#define GLCall(x)                                   \
    ay::gl::GLCheckError();                         \
    x;                                              \
    if (!ay::gl::GLLogCall(#x, __FILE__, __LINE__)) \
        exit(1);

#else

#define GLCall(x) x;

#endif

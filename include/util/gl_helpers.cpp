#include <GL/glew.h>

#include "util/gl_helpers.hpp"
#include "std_header.hpp"

namespace ay {

GLenum GLCheckError()
{
    return glGetError();
}

bool GLLogCall(const char* function, const char* file, int32_t line)
{
    GLenum error = GLCheckError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "[OpenGL Error] (" << error << "): "<< function << " " << file << ":" << line << "\n";
        return false;
    }
    return true;
}

}
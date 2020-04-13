
#include "std_header.hpp"

#include "util/gl_helpers.hpp"

namespace ay::gl
{

GLenum GLCheckError()
{
    return glGetError();
}

bool GLLogCall(const char *function, const char *file, int32_t line)
{
    GLenum error = GLCheckError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file
                  << ":" << line << "\n";
        return false;
    }
    return true;
}

}  // namespace ay::gl

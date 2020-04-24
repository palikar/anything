
#include "std_header.hpp"

#include "util/gl_helpers.hpp"

#include <stb_image_write.h>

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

void take_screenshot(const std::string& file_name)
{
    
    GLint viewport[4];
    GLCall(glGetIntegerv( GL_VIEWPORT, viewport ));
    char* pixels = new char[3 * viewport[2] * viewport[3]];
    GLCall(glReadPixels(viewport[0], viewport[1], viewport[2], viewport[3], GL_RGB, GL_UNSIGNED_BYTE, pixels));
    
    stbi_flip_vertically_on_write(1);
    stbi_write_png(file_name.c_str(), viewport[2], viewport[3], 3, pixels, 0);
    
    delete [] pixels;
}

}  // namespace ay::gl

#include <iostream>

#include <GL/glew.h>

#include "application/application.hpp"
#include "commons.hpp"


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




// int main()
// {k
//     ay::Application app{1024, 768, nullptr};
//     return app.run();
    
//     return 0;
// }

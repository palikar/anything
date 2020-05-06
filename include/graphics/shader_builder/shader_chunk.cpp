#include "shader_chunk.hpp"


namespace ay::shdr
{

void ShaderChunk::init()
{

    if (!m_initialized)
    {
        m_content     = files::load_file(m_file);
        m_initialized = true;
    }
}


}  // namespace ay::shdr

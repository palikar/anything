#pragma once

#include "rendering/buffers.hpp"

#include "util/files.hpp"

#include "std_header.hpp"

#include <fmt/format.h>

namespace ay::shdr
{

enum ChunkStage
{
    VERTEX = 0,
    FRAGMENT
};

class ShaderChunk
{
  private:
    ChunkStage m_stage{ChunkStage::VERTEX};

    std::string m_content;
    std::string m_file;

    std::vector<std::string> m_provides;
    std::vector<std::string> m_requires;
    
    std::vector<std::string> m_defines;

    bool m_initialized{false};

  public:

    ShaderChunk()
    {
    }

    void init()
    {

        if (!m_initialized)
        {
            m_content = files::load_file(m_file);
            m_initialized = true;
        }        
        
    }

    std::vector<std::string> &provides()
    {
        return m_provides;
    }

    std::vector<std::string> &requires_featrues()
    {
        return m_requires;
    }

    std::string &content()
    {
        return m_content;
    }

    std::vector<std::string> &defines()
    {
        return m_defines;
    }

    ChunkStage &stage()
    {
        return m_stage;
    }

    void set_file(std::string t_file)
    {
        m_initialized = false;
        m_file = std::move(t_file);
    }
    
    void set_content(std::string t_content)
    {
        m_initialized = true;
        m_content = std::move(t_content);
    }

    void add_provide(std::string t_feature)
    {
        m_provides.push_back(t_feature);
    }

    void add_requirement(std::string t_feature)
    {
        m_requires.push_back(t_feature);
    }


};


}

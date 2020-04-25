#pragma once

#include "graphics/mesh.hpp"
#include "graphics/geometry.hpp"
#include "graphics/material.hpp"

#include "std_header.hpp"

namespace ay::grph
{

class Model
{

  private:

    std::vector<Mesh> m_meshes;
    
  public:

    explicit Model(std::vector<Mesh> t_meshes) : m_meshes(std::move(t_meshes))
    {
    }


    Model(const Model&) = default;
    Model(Model&&) = default;

    void add_mesh(Mesh t_mesh)
    {
        m_meshes.push_back(std::move(t_mesh));
    }   
    
    size_t size()
    {
        return std::size(m_meshes);
    }

    Mesh* get(size_t t_index)
    {
        return &m_meshes[t_index];
    }    

};

}

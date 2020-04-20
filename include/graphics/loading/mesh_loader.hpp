#pragma once


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "graphics/mesh.hpp"
#include "graphics/geometry.hpp"
#include "graphics/material.hpp"
#include "graphics/material_builder.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::load
{


class Loader
{

  private:
    static grph::Geometry load_mesh(aiMesh *t_mesh)
    {
        grph::Geometry geometry;

        if (t_mesh->HasPositions())
        {

            std::vector<float> verts;
            verts.reserve(t_mesh->mNumVertices);

            for (unsigned int j = 0; j < t_mesh->mNumVertices; j++)
            {

                verts.push_back(t_mesh->mVertices[j].x);
                verts.push_back(t_mesh->mVertices[j].y);
                verts.push_back(t_mesh->mVertices[j].z);
            }

            geometry.set_attribute("position", std::move(verts), 3);
        }

        if (t_mesh->HasNormals())
        {
            std::vector<float> norms;
            norms.reserve(t_mesh->mNumVertices);

            for (unsigned int j = 0; j < t_mesh->mNumVertices; j++)
            {
                norms.push_back(t_mesh->mNormals[j].x);
                norms.push_back(t_mesh->mNormals[j].y);
                norms.push_back(t_mesh->mNormals[j].z);
            }
            geometry.set_attribute("normal", std::move(norms), 3);
        }

        if (t_mesh->HasTextureCoords(0))
        {
            std::vector<float> uvs;
            uvs.reserve(t_mesh->mNumVertices);

            for (unsigned int j = 0; j < t_mesh->mNumVertices; j++)
            {
                uvs.push_back(t_mesh->mTextureCoords[0][j].x);
                uvs.push_back(t_mesh->mTextureCoords[0][j].y);
            }
            geometry.set_attribute("uv", std::move(uvs), 2);
        }

        if (t_mesh->HasFaces())
        {
            std::vector<uint32_t> indices;
            for (unsigned int i = 0; i < t_mesh->mNumFaces; i++)
            {
                aiFace face = t_mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            geometry.set_index(std::move(indices));
        }

        return geometry;
    }

  public:
    static grph::Geometry load_geometry(std::string_view t_path)
    {

        Assimp::Importer importer;
        const aiScene *scene =
          importer.ReadFile(t_path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene)
        {
            std::cout << "No mesh found!"
                      << "\n";
            std::cout << importer.GetErrorString() << "\n";
        }

        if (scene->mNumMeshes > 0)
        {
            aiMesh *mesh = scene->mMeshes[0];
            return load_mesh(mesh);
        }

        return {};
    }
};


}  // namespace ay::load

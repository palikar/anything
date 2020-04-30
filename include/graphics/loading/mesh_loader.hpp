#pragma once


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "graphics/mesh.hpp"
#include "graphics/model.hpp"
#include "graphics/geometry.hpp"

#include "graphics/material.hpp"
#include "graphics/material_builder.hpp"
#include "graphics/materials/solid_color.hpp"
#include "graphics/materials/textured.hpp"
#include "graphics/materials/phong.hpp"


#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::load
{


class Loader
{

  private:
    static grph::Geometry load_geometry(aiMesh *t_mesh)
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

        if (t_mesh->HasTangentsAndBitangents())
        {
            std::vector<float> tangents;
            std::vector<float> bitangents;
            tangents.reserve(t_mesh->mNumVertices);
            bitangents.reserve(t_mesh->mNumVertices);

            for (unsigned int j = 0; j < t_mesh->mNumVertices; j++)
            {
                tangents.push_back(t_mesh->mTangents[j].x);
                tangents.push_back(t_mesh->mTangents[j].y);
                tangents.push_back(t_mesh->mTangents[j].z);

                bitangents.push_back(t_mesh->mBitangents[j].x);
                bitangents.push_back(t_mesh->mBitangents[j].y);
                bitangents.push_back(t_mesh->mBitangents[j].z);
            }

            geometry.set_attribute("tangent", std::move(tangents), 3);
            geometry.set_attribute("bitangent", std::move(bitangents), 3);
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

    static grph::MaterialPtr load_material(aiMaterial *t_mat, const aiScene *)
    {
        aiString name;
        t_mat->Get(AI_MATKEY_NAME, name);
        std::cout << "Loading Material: " << name.C_Str() << "\n";

        auto new_mat     = grph::phong_material(0.0f, 0.0f, 0.0f);
        auto phong_build = grph::PhongMaterialBuilder::from_existing(new_mat.get());
        auto base_build  = grph::MaterialBuilder::from_existing(new_mat.get());

        aiColor3D diffuse_color;
        if (AI_SUCCESS == t_mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color))
        {
            phong_build.color(diffuse_color.r, diffuse_color.g, diffuse_color.b);
        }

        aiColor3D specular_color;
        if (AI_SUCCESS == t_mat->Get(AI_MATKEY_COLOR_SPECULAR, specular_color))
        {
            phong_build.specular(specular_color.r, specular_color.g, specular_color.b);
        }

        aiColor3D ambient_color;
        if (AI_SUCCESS == t_mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color))
        {
            phong_build.ambient(ambient_color.r, ambient_color.g, ambient_color.b);
        }

        float shininess;
        if (AI_SUCCESS == t_mat->Get(AI_MATKEY_SHININESS, shininess))
        {
            phong_build.shininess(shininess);
        }

        aiColor3D emissive_color;
        if (AI_SUCCESS == t_mat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive_color))
        {
            phong_build.emissive(emissive_color.r, emissive_color.g, emissive_color.b);
        }

        aiString tex_dif;
        if (AI_SUCCESS == t_mat->GetTexture(aiTextureType_DIFFUSE, 0, &tex_dif))
        {
            std::cout << "has diffuse tex: " << tex_dif.C_Str() << "\n";
        }

        int wireframe;
        if (AI_SUCCESS == t_mat->Get(AI_MATKEY_ENABLE_WIREFRAME, wireframe))
        {
            base_build.wire_frame(wireframe == 0);
        }

        int culling;
        if (AI_SUCCESS == t_mat->Get(AI_MATKEY_TWOSIDED, culling))
        {
            if (culling == 0)
            {
                base_build.front_side();
            }
            else
            {
                base_build.both_side();
            }
        }

        int blend;
        if (AI_SUCCESS == t_mat->Get(AI_MATKEY_BLEND_FUNC, blend))
        {
            if (blend == aiBlendMode_Default)
            {
                base_build.alpha_blending();
            }
            else if (blend == aiBlendMode_Additive)
            {
                base_build.alpha_blending();
            }
        }

        float opacity;
        if (AI_SUCCESS == t_mat->Get(AI_MATKEY_OPACITY, opacity))
        {
            base_build.opacity(opacity);
        }


        phong_build.lighting(true);

        return new_mat;
    }

  public:
    static grph::Geometry load_geometry(std::string_view t_path)
    {

        Assimp::Importer importer;
        const aiScene *scene =
          importer.ReadFile(t_path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene)
        {
            AY_ERROR("No mesh found!");
            AY_ERROR(importer.GetErrorString());
        }

        if (scene->mNumMeshes > 0)
        {
            aiMesh *mesh = scene->mMeshes[0];
            return load_geometry(mesh);
        }

        return {};
    }

    static grph::Model load_model(std::string_view t_path)
    {

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(
          t_path.data(),
          aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
            | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

        std::vector<grph::Mesh> meshes;

        if (scene->mNumMeshes > 0)
        {
            for (size_t i = 0; i < scene->mNumMeshes; ++i)
            {

                aiMesh *mesh = scene->mMeshes[i];
                auto geom    = load_geometry(mesh);

                auto t_mat =
                  load_material(scene->mMaterials[mesh->mMaterialIndex], scene);

                meshes.push_back({ std::move(geom), std::move(t_mat) });
            }
        }

        return grph::Model(std::move(meshes));
    }
};


}  // namespace ay::load

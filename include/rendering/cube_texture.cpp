#include "rendering/cube_texture.hpp"


namespace ay::rend
{

unsigned char *CubeTexture::load_image(const std::string &t_file)
{
    int width{}, height{}, channels{};
    stbi_uc *data = nullptr;
    data          = stbi_load(t_file.c_str(), &width, &height, &channels, 0);
    m_width       = width;
    m_height      = height;
    return data;
}


void CubeTexture::load_from_files(const std::vector<std::string> &t_files,
                                  TextureFormat t_format)
{


    const std::string &xpos = t_files[0];
    const std::string &xneg = t_files[1];
    const std::string &ypos = t_files[2];
    const std::string &yneg = t_files[3];
    const std::string &zpos = t_files[4];
    const std::string &zneg = t_files[5];


    GLCall(glGenTextures(1, &m_id));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));
    GLCall(glTexParameteri(
               GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    {
        auto xp_data = load_image(xpos);
        auto xn_data = load_image(xneg);
        auto yp_data = load_image(ypos);
        auto yn_data = load_image(yneg);
        auto zp_data = load_image(zpos);
        auto zn_data = load_image(zneg);

        GLenum internal_format = static_cast<GLenum>(t_format);

        // std::cout << xp_data << "\n";
        // std::cout << xn_data << "\n";
        // std::cout << yp_data << "\n";
        // std::cout << yn_data << "\n";
        // std::cout << zp_data << "\n";
        // std::cout << zn_data << "\n";

        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                            0,
                            internal_format,
                            m_width,
                            m_height,
                            0,
                            internal_format,
                            GL_UNSIGNED_BYTE,
                            xp_data));
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                            0,
                            internal_format,
                            m_width,
                            m_height,
                            0,
                            internal_format,
                            GL_UNSIGNED_BYTE,
                            xn_data));

        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                            0,
                            internal_format,
                            m_width,
                            m_height,
                            0,
                            internal_format,
                            GL_UNSIGNED_BYTE,
                            yp_data));
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                            0,
                            internal_format,
                            m_width,
                            m_height,
                            0,
                            internal_format,
                            GL_UNSIGNED_BYTE,
                            yn_data));

        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                            0,
                            internal_format,
                            m_width,
                            m_height,
                            0,
                            internal_format,
                            GL_UNSIGNED_BYTE,
                            zp_data));
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                            0,
                            internal_format,
                            m_width,
                            m_height,
                            0,
                            internal_format,
                            GL_UNSIGNED_BYTE,
                            zn_data));

        GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

        
        stbi_image_free(xp_data);
        stbi_image_free(xn_data);
        stbi_image_free(yp_data);
        stbi_image_free(yn_data);
        stbi_image_free(zp_data);
        stbi_image_free(zn_data);
    }
}

}  // namespace ay::rend

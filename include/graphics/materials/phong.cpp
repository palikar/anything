#include "graphics/materials/phong.hpp"


namespace ay::grph
{

void PhongMaterial::init_shader(rend::ShaderLibrary &t_shader_lib)
{
    m_shader = t_shader_lib.load("phong");
}

void PhongMaterial::update_uniforms(rend::TextureBinder &binder, rend::RenderContext &ctx)
{
    m_shader->set("camera_pos", ctx.camera_pos);
}


}

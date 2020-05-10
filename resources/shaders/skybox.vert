#version 460 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_norm;
layout (location = 2) in vec2 i_uv;

uniform mat4 view_projection_matrix;

out vec3 pos;

void main()
{
    pos = i_pos;
    gl_Position = view_projection_matrix * vec4(i_pos.x, i_pos.y, i_pos.z, 1.0);
    
}

#version 460 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_norm;
layout (location = 2) in vec2 i_uv;

smooth out vec2 uv;
smooth out vec3 pos;
smooth out vec3 normal;

out float fog_depth;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

void main()
{

    uv =  i_uv;
    normal =  mat3(transpose(inverse(model_matrix))) * i_norm;
    pos = vec3(model_matrix * vec4(i_pos, 1.0));

    fog_depth = length(view_matrix * vec4(pos, 1.0));
    
    gl_Position = projection_matrix * vec4(pos, 1.0);
    
}

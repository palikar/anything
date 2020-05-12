#version 460 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_norm;
layout (location = 2) in vec2 i_uv;

out vec2 uv;
out vec3 pos;
out vec3 normal;

uniform mat4 projection_matrix;
uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

out float fog_depth;

void main()
{

    uv =  i_uv;
    normal =  mat3(transpose(inverse(model_matrix))) * i_norm;
    pos = vec3(model_matrix * vec4(i_pos, 1.0));

    fog_depth = -(view_matrix * vec4(pos, 1.0)).z;
    
    gl_Position = view_projection_matrix * vec4(pos, 1.0);
    
}

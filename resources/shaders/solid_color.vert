#version 460 core


layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_norm;
layout (location = 2) in vec2 i_uv;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;


out vec2 uv;
out vec3 pos;
out vec3 norm;

out float fog_depth;

void main()
{
    uv =  i_uv;
    norm =  mat3(transpose(inverse(model_matrix))) * i_norm;
    pos = vec3(model_matrix * vec4(i_pos, 1.0));

    fog_depth = -pos.z;
    
    gl_Position = projection_matrix * (model_matrix * vec4(i_pos.x, i_pos.y, i_pos.z, 1.0));
    
}

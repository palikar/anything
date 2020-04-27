#version 460 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_norm;
layout (location = 2) in vec2 i_uv;
layout (location = 3) in vec3 i_tan;
layout (location = 4) in vec3 i_bitan;


out vec2 uv;
out vec3 pos;
out vec3 norm;
out mat3 TBN;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;

void main()
{

    uv =  i_uv;
    norm =  mat3(transpose(inverse(model_matrix))) * i_norm;
    pos = vec3(model_matrix * vec4(i_pos, 1.0));

    vec3 T = normalize(vec3(model_matrix * vec4(i_tan,   0.0)));
    vec3 B = normalize(vec3(model_matrix * vec4(i_bitan, 0.0)));
    vec3 N = normalize(vec3(model_matrix * vec4(i_norm,  0.0)));
    TBN = mat3(T, B, N);
    
    gl_Position = projection_matrix * vec4(pos, 1.0);
    
}
#version 460 core

layout (location = 0) in vec3 i_pos;


uniform mat4 projection_matrix;
uniform mat4 model_matrix;


void main()
{
    
    gl_Position = projection_matrix * (model_matrix * vec4(i_pos.x, i_pos.y, i_pos.z, 1.0));
    
}

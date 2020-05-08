#version 460 core

layout (location = 0) in vec3 i_pos;
// layout (location = 1) in mat4 i_model;

layout (location = 1) in vec3 i_position;
layout (location = 2) in vec4 i_color;

out float col;

uniform mat4 projection_matrix;


void main() {
    
    mat4 model = mat4(1.0);
    model[3].xyz = i_position;
    vec3 pos = vec3(model * vec4(i_pos, 1.0));
    
    gl_Position = projection_matrix * vec4(pos, 1.0);


}


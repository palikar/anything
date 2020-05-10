#version 460 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_position;
layout (location = 2) in vec4 i_color;

out vec4 col;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_projection_matrix;

void main() {

    col = i_color;
        
    mat4 particle_model = mat4(1.0);
    particle_model[3].xyz = i_position;

    mat4 view = view_matrix;

    view[0][0] = 1.0;
    view[1][0] = 0.0;
    view[2][0] = 0.0;

    view[0][1] = 0.0;
    view[1][1] = 1.0;
    view[2][1] = 0.0;    

    view[0][2] = 0.0;
    view[1][2] = 0.0;
    view[2][2] = 1.0;

    vec4 pos = vec4(projection_matrix * view * particle_model *  vec4(i_pos, 1.0));
    
    gl_Position = pos;


}


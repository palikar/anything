#version 460 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec2 i_uv;
layout (location = 2) in vec4 i_position;
layout (location = 3) in float i_angle;
layout (location = 4) in vec4 i_color;

out vec4 col;
out vec2 uv;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_projection_matrix;

void main() {

    col = i_color;
        
    mat4 particle_model = mat4(1.0);
    particle_model[3].xyz = i_position.xyz;

    mat4 view = view_matrix *  particle_model;
        
    view[0][0] = 1.0  * cos(i_angle) * i_position.w;
    view[0][1] = sin(i_angle) * i_position.w;
    view[0][2] = 0.0;

    view[1][0] = -sin(i_angle) * i_position.w;
    view[1][1] = 1.0 * cos(i_angle) * i_position.w;
    view[1][2] = 0.0;    

    view[2][0] = 0.0;
    view[2][1] = 0.0;
    view[2][2] = 1.0 * i_position.w;
    
    vec4 pos = vec4(projection_matrix * view * vec4(i_pos, 1.0));
    gl_Position = pos;
    
    uv = i_uv;


}


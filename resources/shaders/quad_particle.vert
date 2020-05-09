#version 460 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_position;
layout (location = 2) in vec4 i_color;

out vec4 col;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

void main() {

    col = i_color;
        
    mat4 paritcle_model = mat4(1.0);
    paritcle_model[3].xyz = i_position;
    vec3 pos = vec3(model_matrix * paritcle_model * vec4(i_pos, 1.0));
    
    gl_Position = projection_matrix * vec4(pos, 1.0);


}


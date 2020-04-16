#version 460 core

out vec4 frag_color;

in vec3 pos;

uniform samplerCube skybox;

void main()
{
    frag_color = texture(skybox, pos);
    // frag_color = vec4(0.0, 0.0, 1.0, 1.0);
    
} 

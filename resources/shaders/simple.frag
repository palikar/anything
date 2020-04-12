#version 460 core

out vec4 frag_color;


uniform float red_channel;

void main()
{
    frag_color = vec4(red_channel, 1.0f, 1.0f, 1.0f);
    
} 

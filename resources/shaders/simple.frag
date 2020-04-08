#version 460 core

out vec4 FragColor;


uniform float red_channel;

void main()
{
    FragColor = vec4(red_channel, 0.0f, 1.0f, 1.0f);
    
} 

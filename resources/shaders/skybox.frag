#version 460 core

out vec4 frag_color;

in vec3 pos;

uniform samplerCube skybox;

uniform vec3 fog_color;
uniform int fog_type;

uniform float lower;
uniform float upper;

void main()
{
    frag_color = texture(skybox, pos);
    
    if (fog_type != 0) {
        const float factor = (pos.y - lower) / (upper - lower);
        frag_color = mix(texture(skybox, pos), vec4(fog_color, 1.0), clamp(1.0 - factor, 0.0, 1.0));
    }
    
} 

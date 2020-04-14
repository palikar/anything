#version 460 core

out vec4 frag_color;

uniform sampler2D tex;

in vec2 uv;

void main()
{

    frag_color = texture(tex, uv);
    
}

#version 460 core

out vec4 frag_color;

uniform sampler2D tex;

in vec2 uv;

void main()
{

    frag_color = texture(tex, uv);
    
    // frag_color = vec4(color, 1.0f);
    // frag_color = vec4(1.0, 1.0, 1.0 , 1.0f);
    
}

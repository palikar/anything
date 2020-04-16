#version 460 core

out vec4 frag_color;

uniform vec3 color;
uniform float opacity;
uniform float alpha_threshold;
uniform bool visible;

void main()
{

    if (!visible){
        discard;
    }
    
    if (opacity <= 0.0) {
        frag_color = vec4(color, 1.0);
    } else {
        
        if (alpha_threshold >= 0.0 && opacity < alpha_threshold){
            discard;
        }

        
        frag_color = vec4(color, opacity);
    }

    // frag_color = vec4(1.0, 1.0, 1.0 , 1.0f);
    
}

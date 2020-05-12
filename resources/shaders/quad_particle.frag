#version 460 core

out vec4 frag_color;

in vec4 col;
in vec2 uv;

flat in float factor;
flat in float index;


uniform int width_segments;
uniform int height_segments;

uniform sampler2D atlas;
uniform bool has_atlas;

vec4 get_atlas_color(int i) {

    const float x = float(i % width_segments);
    const float y = height_segments - float(i / width_segments) - 1;

    const float x_offset = (1.0/width_segments) * x;
    const float y_offset = (1.0/height_segments) * y;

    const vec2 atlas_uv = vec2(uv.x/width_segments + x_offset,
                               uv.y/height_segments + y_offset);
    

    return texture(atlas, atlas_uv);

}

void main() {

    const int max_index = width_segments*height_segments - 1;
    
    if (has_atlas) {

        const int i_1 = int(round(index));
        const int i_2 = min(i_1 + 1, max_index);

        const vec4 p_1 = get_atlas_color(i_1);
        const vec4 p_2 = get_atlas_color(i_2);
        
        const float f =  clamp(factor, 0.0, 1.0);
        
        frag_color = mix(p_1, p_2, f);

    } else {
        frag_color = col;

    }

}

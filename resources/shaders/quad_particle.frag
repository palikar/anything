#version 460 core

out vec4 frag_color;

in vec4 col;
in vec2 uv;

uniform int width_segments;
uniform int height_segments;

uniform sampler2D atlas;
uniform bool has_atlas;

void main() {

    if (has_atlas) {

        int x = 3 / width_segments;
        int y = 3 % width_segments;

        vec2 size = textureSize(atlas, 0);

        float x_offset = float(size.x/width_segments) * x;
        float y_offset = float(size.y/height_segments) * y;

        vec2 atlas_uv = vec2(uv.x/width_segments + x_offset,
                             uv.y/height_segments + y_offset);

        
        vec4 p = texture(atlas, atlas_uv);
        frag_color = p;

    } else {
        frag_color = col;

    }
        
}

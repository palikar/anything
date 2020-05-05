vec2 tex_coords = uv;
if (has_height_map) {
    vec3 view_dir  = normalize(tan_view_pos - tan_pos);
    tex_coords = parallax_mapping(tex_coords, view_dir);
}

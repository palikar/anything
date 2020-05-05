vec3 total_emissive = emissive;
if (has_emissive_map) {
    total_emissive *= emissive_scale * texture(emissive_map, tex_coords).rgb;
}

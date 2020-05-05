vec3 diffuse_color = color;
if (has_map) {
    diffuse_color = mix(diffuse_color, texture(map, tex_coords).rgb, 0.5);
}

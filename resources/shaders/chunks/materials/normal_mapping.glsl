vec3 normal = normalize(normal);
if (has_normal_map) {
    normal = texture(normal_map, tex_coords).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(TBN * normal);
}

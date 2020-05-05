vec3 specular_color = specular.rgb;
float specular_strength = 1.0;
float shininess_strength = shininess;
if (has_specular_map) {
    specular_strength = texture(specular_map, tex_coords).r;
}

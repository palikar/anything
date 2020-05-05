if (has_ao_map) {
    float ambient_strength = (texture(ao_map, tex_coords).r - 1.0) * ao_intensity + 1.0;
    outgoing_light *= ambient_strength;
}

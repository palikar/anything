if (fog_type == 1) {
    const float fog_factor = smoothstep(fog_near, fog_far, fog_depth);
    frag_color.rgb = mix(frag_color.rgb, fog_color, fog_factor);
} else if (fog_type == 2) {
    const float fog_factor = 1.0 - exp( - fog_density * fog_density * fog_depth * fog_depth );
    frag_color.rgb = mix(frag_color.rgb, fog_color, fog_factor);
}

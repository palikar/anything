if (has_env) {

    vec3 I = normalize(pos - camera_pos);
    vec3 env_color = vec3(0,0,0);

    if (is_reflection) {
        vec3 R = reflect(I, normalize(normal));
        env_color = texture(env_map, R).rgb;
    } else {
        vec3 R = refract(I, normalize(normal), refraction_ration);
        env_color = texture(env_map, R).rgb;
    }

    outgoing_light = mix(outgoing_light, env_color.rgb, reflectivity * material.specular_strength);
}

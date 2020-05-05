vec3 outgoing_light = vec3(0.0);

if (lighting_enabled) {

    vec3 to_camera = -normalize(pos - camera_pos);

    outgoing_light += apply_ambient_light(lighting.ambient_light, material);
    outgoing_light += apply_dir_light(lighting.dir_light, material, normal , pos, to_camera);

    for (int i = 0; i < MAX_LIGHT; ++i) {
        if(lighting.point_lights[i].act) {
            outgoing_light += apply_point_light(lighting.point_lights[i], material, normal, pos, to_camera);
        }
    }

    for (int i = 0; i < MAX_LIGHT; ++i) {
        if(lighting.spot_lights[i].act) {
            outgoing_light += apply_spot_light(lighting.spot_lights[i], material, normal, pos, to_camera);
        }
    }

} else {
    outgoing_light = material.diffuse;
}

#ifdef HAS_EMISSION
outgoing_light += total_emissive;
#endif


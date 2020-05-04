vec3 apply_dir_light(DirLight light, BlinnPhongMaterial material, vec3 normal, vec3 surface_pos, vec3 surface_to_camera)
{
    vec3 light_to_surface  = normalize(light.direction);
    float diffuse_coefficient = max(0.0, dot(normal, light_to_surface));
    vec3 diffuse = diffuse_coefficient * material.diffuse.rgb * light.color * light.intensity;
    return diffuse;
}

vec3 apply_ambient_light(AmbientLight light, BlinnPhongMaterial material)
{
    return light.intensity * material.ambient.rgb;
}

vec3 apply_point_light(PointLight light, BlinnPhongMaterial material, vec3 normal, vec3 surface_pos, vec3 surface_to_camera)
{

    vec3 light_to_surface  = normalize(light.position - surface_pos);
    float distance_to_light = length(light.position - surface_pos);

    float attenuation = 1.0f / (light.constant + light.linear * distance_to_light +
                                light.quadratic * (distance_to_light * distance_to_light));


    float diffuse_coefficient = max(0.0, dot(normal, light_to_surface));
    vec3 diffuse = diffuse_coefficient * material.diffuse.rgb * light.color;

    float specular_coefficient = 0.0;
    if(diffuse_coefficient > 0.0)
        specular_coefficient = pow(max(0.0, dot(surface_to_camera, reflect(-light_to_surface, normal))), material.specular_shininess);
    vec3 specular = specular_coefficient * material.specular * light.color * material.specular_strength;

    return (diffuse + specular) * attenuation;
}

vec3 apply_spot_light(SpotLight light, BlinnPhongMaterial material, vec3 normal, vec3 surface_pos, vec3 surface_to_camera)
{
    vec3 light_to_surface  = normalize(light.position - surface_pos);
    float theta = acos(dot(light_to_surface, normalize(-light.dir)));

    if(theta < light.outer_cut_off) {
        float epsilon  = light.cut_off - light.outer_cut_off;
        float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
        float distance_to_light = length(light.position - surface_pos);

        float diffuse_coefficient = max(0.0, dot(normal, light_to_surface));
        vec3 diffuse = material.diffuse.rgb * light.color * diffuse_coefficient;

        float specular_coefficient = 0.0;
        if(diffuse_coefficient > 0.0) {
            specular_coefficient = pow(max(0.0, dot(surface_to_camera, reflect(-light_to_surface, normal))), material.specular_shininess);
        }
        vec3 specular = specular_coefficient * material.specular.rgb * light.color * material.specular_strength;

        return (diffuse + specular) * intensity;
    }
    return vec3(0.0);

}

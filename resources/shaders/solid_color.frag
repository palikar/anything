#version 460 core

#define MAX_LIGHT 5

struct DirLight {
    bool act;
    vec3 direction;
    vec3 color;
    float intensity;
};

struct AmbientLight {
    bool act;
    float intensity;
    vec3 color;
};

struct PointLight {
    bool act;

    vec3 color;
    vec3 position;
    
    float linear;
    float constant;
    float quadratic;
};

struct SpotLight {
    bool act;

    vec3 color;
    vec3 position;
    vec3 dir;
    
    float cut_off;
    float outer_cut_off;
};

struct LightSetup {
    DirLight dir_light;

    AmbientLight ambient_light;

    PointLight point_lights[MAX_LIGHT];
    SpotLight spot_lights[MAX_LIGHT];
};

out vec4 frag_color;

in vec2 uv;
in vec3 pos;
in vec3 norm;

in float fog_depth;

uniform vec3 color;
uniform float opacity;
uniform float alpha_threshold;
uniform bool visible;

uniform float specular_exponent;

uniform vec3 camera_pos;

uniform bool lighting_enabled;
uniform LightSetup lighting;


vec3 apply_dir_light(DirLight light, vec3 surface_color, vec3 normal, vec3 surface_pos, vec3 surface_to_camera)
{
    vec3 light_to_surface  = normalize(light.direction);
    float diffuse_coefficient = max(0.0, dot(normal, light_to_surface));
    vec3 diffuse = diffuse_coefficient * surface_color.rgb * light.color * light.intensity;

    return diffuse;
}

vec3 apply_ambient_light(AmbientLight light, vec3 surface_color)
{
    return light.intensity * surface_color.rgb;
}

vec3 apply_point_light(PointLight light, vec3 surface_color, vec3 normal, vec3 surface_pos, vec3 surface_to_camera)
{

    vec3 light_to_surface  = normalize(light.position - surface_pos);
    float distance_to_light = length(light.position - surface_pos);

    float attenuation = 1.0f / (light.constant + light.linear * distance_to_light + 
                                light.quadratic * (distance_to_light * distance_to_light));

    
    float diffuse_coefficient = max(0.0, dot(normal, light_to_surface));
    vec3 diffuse = diffuse_coefficient * surface_color.rgb * light.color;

    float specular_coefficient = 0.0;
    if(diffuse_coefficient > 0.0)
        specular_coefficient = pow(max(0.0, dot(surface_to_camera, reflect(-light_to_surface, normal))), specular_exponent);
    vec3 specular = specular_coefficient * surface_color.rgb * light.color;

    return (diffuse + specular) * attenuation;
}

vec3 apply_spot_light(SpotLight light, vec3 surface_color, vec3 normal, vec3 surface_pos, vec3 surface_to_camera)
{
    vec3 light_to_surface  = normalize(light.position - surface_pos);
    float theta = acos(dot(light_to_surface, normalize(-light.dir)));
    
    if(theta < light.outer_cut_off) {
        float epsilon  = light.cut_off - light.outer_cut_off;
        float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
        float distance_to_light = length(light.position - surface_pos);        

        float diffuse_coefficient = max(0.0, dot(normal, light_to_surface));
        vec3 diffuse = surface_color.rgb * light.color * diffuse_coefficient;
        
        float specular_coefficient = 0.0;
        if(diffuse_coefficient > 0.0) {
            specular_coefficient = pow(max(0.0, dot(surface_to_camera, reflect(-light_to_surface, normal))), specular_exponent);
        }
        vec3 specular = specular_coefficient * surface_color.rgb * light.color;

        return (diffuse + specular) * intensity;
    }
    return vec3(0.0);

}

void main()
{

    if (!visible){
        discard;
    }

    if (opacity <= 0.0) {
        frag_color = vec4(color, 1.0);
    } else {
        if (alpha_threshold >= 0.0 && opacity < alpha_threshold){
            discard;
        }
        frag_color = vec4(color, opacity);
    }

    if (lighting_enabled) {

        vec3 normal = normalize(norm);
        vec3 to_camera = - normalize(pos - camera_pos);

        vec3 final_color = vec3(0.0);
    
        final_color += apply_ambient_light(lighting.ambient_light, frag_color.rgb);
        final_color += apply_dir_light(lighting.dir_light, frag_color.rgb, normal , pos, to_camera);
    
        for (int i = 0; i < MAX_LIGHT; ++i) {
            if(lighting.point_lights[i].act) {
                final_color += apply_point_light(lighting.point_lights[i], frag_color.rgb, normal, pos, to_camera);   
            }
        }

        for (int i = 0; i < MAX_LIGHT; ++i) {
            if(lighting.spot_lights[i].act) {
                final_color += apply_spot_light(lighting.spot_lights[i], frag_color.rgb, normal, pos, to_camera);   
            }
        }
        
        frag_color = vec4(final_color.rgb, opacity);
    }

    // vec3 fog_color = vecv(0.2,0.2, 0.2);
    // float fogFactor = 1.0 - exp( - fog_density * fog_density * fog_depth * fog_depth );
    // frag_color.rgb = mix( rag_color.rgb, fog_color, fog_color);

}

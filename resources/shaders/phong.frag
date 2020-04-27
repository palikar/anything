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

struct BlinnPhongMaterial {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float specular_shininess;
    float specular_strength;
};

// Output
out vec4 frag_color;

// Input
in vec2 uv;
in vec3 pos;
in vec3 norm;


// Phong material
uniform vec3 camera_pos;

uniform vec3 color;
uniform vec3 ambient;

uniform bool has_ao_map;
uniform float ao_intensity;
uniform sampler2D ao_map;

uniform float bump_scale;
uniform sampler2D bump_map;

uniform float disp_scale;
uniform float disp_bias;
uniform sampler2D displ_map;

uniform vec3 emissive;
uniform float emissive_scale;
uniform bool has_emissive_map;
uniform sampler2D emissive_map;

uniform bool has_env;
uniform samplerCube env_map;

uniform bool has_map;
uniform bool is_reflection;
uniform float reflectivity;
uniform float refraction_ration;
uniform sampler2D map;

uniform sampler2D normal_map;

uniform float shininess;
uniform vec3 specular;
uniform bool has_specular_map;
uniform sampler2D specular_map;


// Basic  material
uniform float opacity;
uniform float alpha_threshold;
uniform bool visible;


// Lighting
uniform bool lighting_enabled;
uniform LightSetup lighting;


vec3 apply_dir_light(DirLight light, BlinnPhongMaterial material, vec3 normal, vec3 surface_pos, vec3 surface_to_camera)
{
    vec3 light_to_surface  = normalize(light.direction);
    float diffuse_coefficient = max(0.0, dot(normal, light_to_surface));
    vec3 diffuse = diffuse_coefficient * material.specular.rgb * light.color * light.intensity;
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

    vec3 diffuse_color = color;
    vec3 total_emissive = emissive;

    if (has_map) {
        diffuse_color = mix(diffuse_color, texture(map, uv).rgb, 0.5);
    }

    float specular_strength = 1.0;
    if (has_specular_map) {
        specular_strength = texture(specular_map, uv).r;
    }

    if (has_emissive_map) {
        total_emissive *= texture(emissive_map, uv).rgb;
    }

    BlinnPhongMaterial material;
    material.ambient = ambient.rgb;
    material.diffuse = diffuse_color.rgb;
    material.specular = specular.rgb;
    material.specular_shininess = shininess;
    material.specular_strength = specular_strength;

    vec3 outgoing_light = vec3(0.0);

    if (lighting_enabled) {
        vec3 normal = normalize(norm);
        vec3 to_camera = - normalize(pos - camera_pos);        

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

    if (has_ao_map) {
        float ambient_strength = (texture(ao_map, uv).r - 1.0) * ao_intensity + 1.0;
        outgoing_light *= ambient_strength;
    }

    if (has_env) {

        vec3 I = normalize(pos - camera_pos);
        vec3 env_color = vec3(0,0,0);

        if (is_relfection) {
            vec3 R = reflect(I, normalize(normal));
            env_color = texture(env_map, R).rgb;
        } else {
            vec3 R = refract(I, normalize(normal), refraction_ration);
            env_color = texture(env_map, R).rgb;
        }
        
        outgoing_light = mix(outgoing_light, env_color.rgb, reflectivity * material.specular_strength) ;
    }

    

    
    
    frag_color = vec4(diffuse_color.xyz, opacity);

}

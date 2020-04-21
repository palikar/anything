#version 460 core

out vec4 frag_color;

uniform vec3 color;
uniform float opacity;
uniform float alpha_threshold;
uniform bool visible;


struct DirLight {
    bool act;
    vec3 direction;
    vec3 color;
    float intensity;
};


struct LightSetup
{
    DirLight dir_light;
    
};

uniform vec3 camera_pos;

uniform LightSetup lighting;

in vec2 uv;
in vec3 pos;
in vec3 norm;

vec3 apply_dir_light(DirLight light, vec4 surface_color, vec3 normal, vec3 surface_pos, vec3 surface_to_camera)
{
    vec3 surface_to_light;
    float attenuation = 2.0;
    surface_to_light = normalize(light.direction);

    float diffuse_coefficient = max(0.0, dot(normal, surface_to_light));
    vec3 diffuse = diffuse_coefficient * surface_color.rgb * light.color;

    float specular_coefficient = 0.0;
    if(diffuse_coefficient > 0.0) {
        specular_coefficient = pow(max(0.0, dot(surface_to_camera, reflect(-surface_to_light, normal))), 1.2);
    }
    vec3 specular = specular_coefficient  * light.intensity * vec3(0.5,0.5,0.5);

    return diffuse + specular;
    
    
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

    // vec3 normal = normalize(norm);
    // vec3 to_camera = normalize(pos - camera_pos);
    // frag_color = vec4(apply_dir_light(lighting.dir_light, frag_color, normal , pos, to_camera).xyz, opacity);


}
